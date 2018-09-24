#include "type_reader/type_reader.h"
#include "gfakeparam.h"

int main(int argc, char *argv[])
{
    GParamSpec *fake_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary;
    gchar *type_string = NULL;
    gst_init(&argc, &argv);

    dictionary = gst_structure_new_empty("test");

    fake_spec = g_param_spec_fake("test", "Test param", "Test param", G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(fake_spec);
    type_string = g_strdup_printf("Unknown type %ld \"%s\"", (glong)fake_spec->value_type,
                                  g_type_name(fake_spec->value_type));
    gst_type_reader_fill_type(fake_spec, value, dictionary);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_TYPE), ==, type_string);

    g_free(type_string);
    gst_structure_free(dictionary);
    g_param_spec_unref(fake_spec);

    return 0;
}
