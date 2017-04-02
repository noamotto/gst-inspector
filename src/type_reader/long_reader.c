#include "type_reader/type_reader_priv.h"

void gst_long_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;
    GParamSpecLong *pspec_long = NULL;

    g_return_if_fail(G_VALUE_HOLDS_LONG(value));
    g_return_if_fail(G_IS_PARAM_SPEC_LONG(pspec));

    pspec_long = G_PARAM_SPEC_LONG(pspec);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup("Long"));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value,
                        g_strdup_printf("%ld - %ld", pspec_long->minimum,
                                        pspec_long->maximum));
    gst_structure_take_value(dictionary, KEY_RANGE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("%ld",
                                                    g_value_get_long(value)));
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}
