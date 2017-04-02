#include "type_reader/type_reader_priv.h"

void gst_int_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;
    GParamSpecInt *pspec_int = NULL;

    g_return_if_fail(G_VALUE_HOLDS_INT(value));
    g_return_if_fail(G_IS_PARAM_SPEC_INT(pspec));

    pspec_int = G_PARAM_SPEC_INT(pspec);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup("Integer"));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("%d - %d",
                                                    pspec_int->minimum,
                                                    pspec_int->maximum));
    gst_structure_take_value(dictionary, KEY_RANGE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("%d", g_value_get_int(value)));
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}
