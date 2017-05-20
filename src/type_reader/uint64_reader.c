#include "type_reader/type_reader_priv.h"

void gst_uint64_type_reader_fill_type(
    GParamSpec *pspec,
    GValue *value,
    GstStructure *dictionary)
{
    GValue key_value = G_VALUE_INIT;
    GParamSpecUInt64 *pspec_uint64 = NULL;

    g_return_if_fail(G_VALUE_HOLDS_UINT64(value));
    g_return_if_fail(G_IS_PARAM_SPEC_UINT64(pspec));

    pspec_uint64 = G_PARAM_SPEC_UINT64(pspec);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup("Unsigned Integer64"));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value,
                        g_strdup_printf("%" G_GUINT64_FORMAT " - %" G_GUINT64_FORMAT,
                                        pspec_uint64->minimum, pspec_uint64->maximum));
    gst_structure_take_value(dictionary, KEY_RANGE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("%" G_GUINT64_FORMAT,
                                                    g_value_get_uint64(value)));
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}
