#include "type_reader/type_reader.h"

#define MINIMUM ((gdouble)G_MINFLOAT)
#define MAXIMUM ((gdouble)G_MAXFLOAT)
#define VALUE ((gdouble)(G_MAXFLOAT / 2))

int main(int argc, char *argv[])
{
    GParamSpec *float_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary;
    gchar *range_string = NULL;
    gchar *value_string = NULL;

    gst_init(&argc, &argv);

    dictionary = gst_structure_new_empty("test");
    float_spec = g_param_spec_float("test", "Test param",
                                    "Test param", MINIMUM, MAXIMUM, VALUE, G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(float_spec);
    range_string = g_strdup_printf("%15.7g - %15.7g", MINIMUM, MAXIMUM);
    value_string = g_strdup_printf("%15.7g", VALUE);

    gst_type_reader_fill_type(float_spec, value, dictionary);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_TYPE), ==, "Float");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_RANGE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_RANGE), ==, range_string);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_VALUE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_VALUE), ==, value_string);

    g_free(range_string);
    g_free(value_string);
    gst_structure_free(dictionary);
    g_param_spec_unref(float_spec);
    return 0;
}
