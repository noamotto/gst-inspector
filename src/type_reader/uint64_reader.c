#include "type_reader/type_reader_priv.h"

void gst_uint64_type_reader_fill_type(
    GParamSpec *pspec,
    GValue *value,
    GstStructure *dictionary)
{
    GParamSpecUInt64 *pspec_uint64 = NULL;

    g_return_if_fail(G_VALUE_HOLDS_UINT64(value));
    g_return_if_fail(G_IS_PARAM_SPEC_UINT64(pspec));

    pspec_uint64 = G_PARAM_SPEC_UINT64(pspec);

    gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Unsigned Integer64");

    gst_dictionary_set_string(dictionary, KEY_RANGE,
                              g_strdup_printf("%" G_GUINT64_FORMAT " - %" G_GUINT64_FORMAT,
                                              pspec_uint64->minimum,
                                              pspec_uint64->maximum));

    gst_dictionary_set_string(dictionary, KEY_VALUE,
                              g_strdup_printf("%" G_GUINT64_FORMAT, g_value_get_uint64(value)));
}
