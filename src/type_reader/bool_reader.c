#include "type_reader/type_reader_priv.h"

void gst_bool_type_reader_fill_type(
    GParamSpec *pspec,
    GValue *value,
    GstStructure *dictionary)
{
    gboolean bool_val;

    g_return_if_fail(G_IS_PARAM_SPEC_BOOLEAN(pspec));
    g_return_if_fail(G_VALUE_HOLDS_BOOLEAN(value));

    gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Boolean");
    
    bool_val = g_value_get_boolean(value);
    gst_dictionary_set_static_string(dictionary, KEY_VALUE, bool_val ? "true" : "false");
}
