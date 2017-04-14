#include "type_reader/type_reader.h"

#define MINIMUM (G_MINFLOAT)
#define MAXIMUM (G_MAXFLOAT)
#define VALUE (G_MAXFLOAT / 2)

int main(int argc, char *argv[])
{
    GParamSpec *float_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary = NULL;
    gchar *range_string = NULL;
    gchar *value_string = NULL;

    gst_init(&argc, &argv);

    float_spec = g_param_spec_float("test", "Test param",
                                "Test param", MINIMUM, MAXIMUM, VALUE, G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(float_spec);
    dictionary = gst_structure_new_empty("dictionary");
    range_string = g_strdup_printf("%15.7g - %15.7g", MINIMUM, MAXIMUM);
    value_string = g_strdup_printf("%15.7g", VALUE);

    gst_type_reader_fill_type(float_spec, (GValue * const)value, dictionary);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_NAME), "test: Test param"),
                         1);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_TYPE), "Float"),
                         1);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_RANGE, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_RANGE), range_string),
                         1);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_VALUE, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_VALUE), value_string),
                         1);

    g_free(range_string);
    g_free(value_string);
    return 0;
}