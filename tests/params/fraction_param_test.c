#include "type_reader/type_reader.h"

#define MINIMUM 1, 1
#define MAXIMUM 1000, 1
#define VALUE 30, 1

int main(int argc, char *argv[])
{
    GParamSpec *int64_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary = NULL;
    gchar *range_string = NULL;
    gchar *value_string = NULL;

    gst_init(&argc, &argv);

    int64_spec = gst_param_spec_fraction("test", "Test param",
                                "Test param", MINIMUM, MAXIMUM, VALUE, G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(int64_spec);
    range_string = g_strdup_printf("%d/%d - %d/%d", MINIMUM, MAXIMUM);
    value_string = g_strdup_printf("%d/%d", VALUE);

    dictionary = gst_type_reader_fill_type(int64_spec, value);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_TYPE), ==, "Fraction");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_RANGE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_RANGE), ==, range_string);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_VALUE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_VALUE), ==, value_string);

    g_free(range_string);
    g_free(value_string);
    return 0;
}
