#include "type_reader/type_reader_priv.h"

void gst_double_type_reader_fill_type(
    GParamSpec *pspec,
    GValue *value,
    GstStructure *dictionary)
{
    gdouble double_val = 0;
    GParamSpecDouble *pspec_double = NULL;

    g_return_if_fail(G_VALUE_HOLDS_DOUBLE(value));
    g_return_if_fail(G_IS_PARAM_SPEC_DOUBLE(pspec));

    pspec_double = G_PARAM_SPEC_DOUBLE(pspec);
    double_val = g_value_get_double(value);

    gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Double");

    gst_dictionary_set_string(dictionary, KEY_RANGE,
                              g_strdup_printf("%15.7g - %15.7g",
                                              pspec_double->minimum, pspec_double->maximum));

    gst_dictionary_set_string(dictionary, KEY_VALUE, g_strdup_printf("%15.7g",
                                                                     g_value_get_double(value)));
}
