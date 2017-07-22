#include "type_reader/type_reader_priv.h"

void gst_int64_type_reader_fill_type(
    GParamSpec *pspec,
    GValue *value,
    GstStructure *dictionary)
{
    GParamSpecInt64 *pspec_int64 = NULL;

    g_return_if_fail(G_VALUE_HOLDS_INT64(value));
    g_return_if_fail(G_IS_PARAM_SPEC_INT64(pspec));

    pspec_int64 = G_PARAM_SPEC_INT64(pspec);

    gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Integer64");

    gst_dictionary_set_string(dictionary, KEY_RANGE,
                              g_strdup_printf("%" G_GINT64_FORMAT " - %" G_GINT64_FORMAT,
                                              pspec_int64->minimum,
                                              pspec_int64->maximum));

    gst_dictionary_set_string(dictionary, KEY_VALUE, g_strdup_printf("%" G_GINT64_FORMAT,
                                                                     g_value_get_int64(value)));
}
