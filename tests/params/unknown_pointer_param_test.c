#include "type_reader/type_reader.h"

int main(int argc, char *argv[])
{
    GParamSpec *pointer_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary;

    gst_init(&argc, &argv);

    dictionary = gst_structure_new_empty("test");
    pointer_spec = g_param_spec_pointer("test", "Test param",
                                     "Test param", G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(pointer_spec);

    gst_type_reader_fill_type(pointer_spec, value, dictionary);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dictionary, KEY_TYPE), ==, "Pointer.");

    gst_structure_free(dictionary);
    g_param_spec_unref(pointer_spec);
    gst_deinit();

    return 0;
}
