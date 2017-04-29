#include "type_reader/type_reader.h"

int main(int argc, char *argv[])
{
    GParamSpec *string_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary = NULL;

    gst_init(&argc, &argv);

    string_spec = g_param_spec_string("test", "Test param",
                                                 "Test param", "Hello", G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(string_spec);
    dictionary = gst_structure_new_empty("dictionary");

    gst_type_reader_fill_type(string_spec, (GValue * const)value, dictionary);
    
    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_NAME), "test: Test param"),
                         1);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_TYPE), "String"),
                         1);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_VALUE, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_VALUE), "Hello"),
                         1);

    return 0;
}