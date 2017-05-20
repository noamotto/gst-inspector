#include "type_reader/type_reader.h"

#define MINIMUM (0)
#define MAXIMUM (G_MAXUINT)
#define VALUE ((G_MAXUINT - 1) / 2)

int main(int argc, char *argv[])
{
    GParamSpec *uint_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary = NULL;
    gchar *range_string = NULL;
    gchar *value_string = NULL;

    gst_init(&argc, &argv);

    uint_spec = g_param_spec_uint("test", "Test param",
                                "Test param", MINIMUM, MAXIMUM, VALUE, G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(uint_spec);
    range_string = g_strdup_printf("%u - %u", MINIMUM, MAXIMUM);
    value_string = g_strdup_printf("%u", VALUE);

    dictionary = gst_type_reader_fill_type(uint_spec, value);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_TYPE), ==, "Unsigned Integer");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_RANGE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_RANGE), ==, range_string);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_VALUE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_VALUE), ==, value_string);

    g_free(range_string);
    g_free(value_string);
    return 0;
}
