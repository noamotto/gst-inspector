#include "type_reader/type_reader.h"
#include "gfakeboxed.h"

int main(int argc, char *argv[])
{
    GParamSpec *boxed_spec = NULL;
    GValue value = G_VALUE_INIT;
    GstStructure *dictionary;
    gchar *type_string = NULL;
    GstStructure *structure = NULL;
    const GValue *fields_array;

    gst_init(&argc, &argv);

    dictionary = gst_structure_new_empty("test");
    type_string = g_strdup_printf("Boxed pointer of type \"%s\"",
                                  g_type_name(GST_TYPE_STRUCTURE));
    boxed_spec = g_param_spec_boxed("test", "Test param",
                                    "Test param", GST_TYPE_STRUCTURE, G_PARAM_READWRITE);

    g_value_init(&value, GST_TYPE_STRUCTURE);
    g_value_take_boxed(&value, structure);

    gst_type_reader_fill_type(boxed_spec, &value, dictionary);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dictionary, KEY_TYPE), ==, type_string);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_VALUE, GST_TYPE_LIST));
    fields_array = gst_dictionary_get_array(dictionary, KEY_VALUE);
    g_assert_cmpuint(gst_array_get_size(fields_array), ==, 0);

    g_free(type_string);
    g_param_spec_unref(boxed_spec);
    g_value_unset(&value);
    gst_structure_free(dictionary);
    return 0;
}
