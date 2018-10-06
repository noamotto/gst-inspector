#include "type_reader/type_reader.h"
#include "gfakeboxed.h"

#define KEY_FEATURES ("Features")

#define CAPS ("video/x-raw, format=RGB, width=320, height=240, framerate=30/1")

static const char *parsed_type = "video/x-raw";
static const char *parsed_fields[] = {"format: RGB",
                                      "width: 320",
                                      "height: 240",
                                      "framerate: 30/1",
                                      NULL};

static void check_parse(const GValue *value)
{
    GArray *caps_array = g_value_get_boxed(value);
    GArray *fields_array = NULL;
    const GValue *caps_value;
    const GstStructure *caps_struct;
    g_assert_cmpuint(caps_array->len, ==, 1);

    caps_value = &g_array_index(caps_array, GValue, 0);
    g_assert_true(g_type_is_a(caps_value->g_type, GST_TYPE_STRUCTURE));

    caps_struct = gst_value_get_structure(caps_value);
    g_assert_true(gst_structure_has_field_typed(caps_struct, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(caps_struct, KEY_TYPE), ==, parsed_type);

    g_assert_false(gst_structure_has_field_typed(caps_struct, KEY_FEATURES, G_TYPE_STRING));

    g_assert_true(gst_structure_has_field_typed(caps_struct, "Caps", G_TYPE_ARRAY));
    fields_array = g_value_get_boxed(gst_structure_get_value(caps_struct, "Caps"));
    for (guint i = 0; i < fields_array->len; i++)
    {
        const GValue *field_value = &g_array_index(fields_array, GValue, i);

        g_assert_true(g_type_is_a(field_value->g_type, G_TYPE_STRING));

        g_assert_cmpstr(g_value_get_string(field_value), ==, parsed_fields[i]);
    }
}

int main(int argc, char *argv[])
{
    GParamSpec *boxed_spec = NULL;
    GValue value = G_VALUE_INIT;
    GstStructure *dictionary;
    gchar *type_string = NULL;
    GstCaps *caps = NULL;

    gst_init(&argc, &argv);

    dictionary = gst_structure_new_empty("test");
    caps = gst_caps_from_string(CAPS);
    gst_caps_set_features(caps, 0,
                          gst_caps_features_from_string(GST_CAPS_FEATURE_MEMORY_SYSTEM_MEMORY));

    type_string = g_strdup_printf("Boxed pointer of type \"%s\"",
                                  g_type_name(GST_TYPE_CAPS));
    boxed_spec = g_param_spec_boxed("test", "Test param",
                                    "Test param", GST_TYPE_CAPS, G_PARAM_READWRITE);

    g_value_init(&value, GST_TYPE_CAPS);
    gst_value_set_caps(&value, caps);

    gst_type_reader_fill_type(boxed_spec, &value, dictionary);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_TYPE), ==, type_string);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_VALUE, G_TYPE_ARRAY));

    check_parse(gst_structure_get_value(dictionary, KEY_VALUE));

    g_free(type_string);
    return 0;
}
