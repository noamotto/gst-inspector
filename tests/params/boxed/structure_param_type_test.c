#include "type_reader/type_reader.h"
#include "gfakeboxed.h"

#define STRUCTURE ("struct, a=1, b=abba, c=5/2")

static const char *parsed_fields[] = {"a: 1",
                                      "b: abba",
                                      "c: 5/2",
                                      NULL};

int main(int argc, char *argv[])
{
    GParamSpec *boxed_spec = NULL;
    GValue value = G_VALUE_INIT;
    GstStructure *dictionary;
    gchar *type_string = NULL;
    GstStructure *structure = NULL;
    GArray *fields_array = NULL;

    gst_init(&argc, &argv);

    dictionary = gst_structure_new_empty("test");
    structure = gst_structure_from_string(STRUCTURE, NULL);
    type_string = g_strdup_printf("Boxed pointer of type \"%s\"",
                                  g_type_name(GST_TYPE_STRUCTURE));
    boxed_spec = g_param_spec_boxed("test", "Test param",
                                    "Test param", GST_TYPE_STRUCTURE, G_PARAM_READWRITE);

    g_value_init(&value, GST_TYPE_STRUCTURE);
    g_value_take_boxed(&value, structure);

    gst_type_reader_fill_type(boxed_spec, &value, dictionary);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_TYPE), ==, type_string);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_VALUE, G_TYPE_ARRAY));
    fields_array = g_value_get_boxed(gst_structure_get_value(dictionary, KEY_VALUE));
    for (guint i = 0; i < fields_array->len; i++)
    {
        const GValue *field_value = &g_array_index(fields_array, GValue, i);

        g_assert_true(g_type_is_a(field_value->g_type, G_TYPE_STRING));

        g_assert_cmpstr(g_value_get_string(field_value), ==, parsed_fields[i]);
    }

    g_free(type_string);
    g_param_spec_unref(boxed_spec);
    g_value_unset(&value);
    gst_structure_free(dictionary);
    return 0;
}
