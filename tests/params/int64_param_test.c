#include "type_reader/type_reader.h"

#define MINIMUM (G_MININT64)
#define MAXIMUM (G_MAXINT64)
#define VALUE (G_GINT64_CONSTANT(0))

int main(int argc, char *argv[])
{
    GParamSpec *int64_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary = NULL;
    gchar *range_string = NULL;
    gchar *value_string = NULL;

    gst_init(&argc, &argv);

    int64_spec = g_param_spec_int64("test", "Test param",
                                "Test param", MINIMUM, MAXIMUM, VALUE, G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(int64_spec);
    dictionary = gst_structure_new_empty("dictionary");
    range_string = g_strdup_printf("%" G_GINT64_FORMAT " - %" G_GINT64_FORMAT, MINIMUM, MAXIMUM);
    value_string = g_strdup_printf("%" G_GINT64_FORMAT, VALUE);

    gst_type_reader_fill_type(int64_spec, (GValue * const)value, dictionary);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_NAME), "test: Test param"),
                         1);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_TYPE), "Integer64"),
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