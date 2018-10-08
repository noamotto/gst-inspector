#include "type_reader/type_reader.h"

int main(int argc, char *argv[])
{
    GParamSpecValueArray *value_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary;
    gchar *type_string;

    gst_init(&argc, &argv);

    dictionary = gst_structure_new_empty("test");
    value_spec = G_PARAM_SPEC_VALUE_ARRAY(g_param_spec_value_array(
        "test", "Test param", "Test param",
        g_param_spec_boolean("param", "param", "param", TRUE, G_PARAM_READWRITE),
        G_PARAM_READWRITE));

    type_string = g_strdup_printf("Array of GValues of type \"%s\"",
                                  g_type_name(value_spec->element_spec->value_type));

    value = g_param_spec_get_default_value(G_PARAM_SPEC(value_spec));

    gst_type_reader_fill_type(G_PARAM_SPEC(value_spec), value, dictionary);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_TYPE), ==, type_string);

    g_free(type_string);
    gst_structure_free(dictionary);
    g_param_spec_unref(G_PARAM_SPEC(value_spec));
    return 0;
}
