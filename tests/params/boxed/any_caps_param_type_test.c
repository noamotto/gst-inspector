#include "type_reader/type_reader.h"
#include "gfakeboxed.h"

int main(int argc, char *argv[])
{
    GParamSpec *boxed_spec = NULL;
    GValue value = G_VALUE_INIT;
    GstStructure *dictionary = NULL;
    gchar *type_string = NULL;
    GstCaps *caps = NULL;

    gst_init(&argc, &argv);

    caps = gst_caps_new_any();
    type_string = g_strdup_printf("Boxed pointer of type \"%s\"",
                                  g_type_name(GST_TYPE_CAPS));
    boxed_spec = g_param_spec_boxed("test", "Test param",
                                    "Test param", GST_TYPE_CAPS, G_PARAM_READWRITE);

    g_value_init(&value, GST_TYPE_CAPS);
    gst_value_set_caps(&value, caps);

    dictionary = gst_type_reader_fill_type(boxed_spec, &value);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_TYPE), ==, type_string);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_VALUE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_VALUE), ==, "ANY");

    g_free(type_string);
    return 0;
}
