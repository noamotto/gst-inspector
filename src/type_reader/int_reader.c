#include "type_reader/type_reader_priv.h"

void gst_int_type_reader_fill_type(
    GParamSpec *pspec,
    GValue *value,
    GstStructure *dictionary)
{
    GParamSpecInt *pspec_int = NULL;

    g_return_if_fail(G_VALUE_HOLDS_INT(value));
    g_return_if_fail(G_IS_PARAM_SPEC_INT(pspec));

    pspec_int = G_PARAM_SPEC_INT(pspec);

    gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Integer");

    gst_dictionary_set_string(dictionary, KEY_RANGE,
                              g_strdup_printf("%d - %d",
                                              pspec_int->minimum,
                                              pspec_int->maximum));

    gst_dictionary_set_string(dictionary, KEY_VALUE,
                              g_strdup_printf("%d", g_value_get_int(value)));
}
