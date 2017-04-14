#include "type_reader/type_reader.h"

#define MINIMUM (0)
#define MAXIMUM (G_MAXULONG)
#define VALUE ((G_MAXULONG - 1) / 2)

int main(int argc, char *argv[])
{
    GParamSpec *ulong_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary = NULL;
    gchar *range_string = NULL;
    gchar *value_string = NULL;

    gst_init(&argc, &argv);

    ulong_spec = g_param_spec_ulong("test", "Test param",
                                "Test param", MINIMUM, MAXIMUM, VALUE, G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(ulong_spec);
    dictionary = gst_structure_new_empty("dictionary");
    range_string = g_strdup_printf("%lu - %lu", MINIMUM, MAXIMUM);
    value_string = g_strdup_printf("%lu", VALUE);

    gst_type_reader_fill_type(ulong_spec, (GValue * const)value, dictionary);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_NAME), "test: Test param"),
                         1);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_TYPE), "Unsigned Long"),
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