#include "type_reader/type_reader.h"

int main(int argc, char *argv[])
{
    GParamSpec *bool_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary;

    gst_init(&argc, &argv);

    dictionary = gst_structure_new_empty("test");
    bool_spec = g_param_spec_boolean("test", "Test param",
                                     "Test param", TRUE, G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(bool_spec);

    gst_type_reader_fill_type(bool_spec, value, dictionary);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_TYPE), ==, "Boolean");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_VALUE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_VALUE), ==, "true");

    gst_structure_free(dictionary);
    g_param_spec_unref(bool_spec);
    return 0;
}
