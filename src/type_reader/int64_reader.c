#include "type_reader/type_reader_priv.h"

void gst_int64_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;
    GParamSpecInt64 *pspec_int64 = NULL;

    g_return_if_fail(G_VALUE_HOLDS_INT64(value));
    g_return_if_fail(G_IS_PARAM_SPEC_INT64(pspec));

    pspec_int64 = G_PARAM_SPEC_INT64(pspec);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup("Integer64"));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value,
                        g_strdup_printf("%" G_GINT64_FORMAT " - %" G_GINT64_FORMAT,
                                        pspec_int64->minimum,
                                        pspec_int64->maximum));
    gst_structure_take_value(dictionary, KEY_RANGE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("%" G_GINT64_FORMAT,
                                                    g_value_get_int64(value)));
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}
