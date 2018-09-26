#include "type_reader/type_reader_priv.h"

void gst_value_array_type_reader_fill_type(
    GParamSpec *pspec,
    GValue *value,
    GstStructure *dictionary)
{
    GParamSpecValueArray *value_array;

    g_return_if_fail(G_IS_PARAM_SPEC_VALUE_ARRAY(pspec));
    g_return_if_fail(G_IS_VALUE(value));

    value_array = G_PARAM_SPEC_VALUE_ARRAY(pspec);

    if (value_array->element_spec)
    {
        gst_dictionary_set_string(dictionary, KEY_TYPE,
                                  g_strdup_printf("Array of GValues of type \"%s\"",
                                                  g_type_name(value_array->element_spec->value_type)));
    }
    else
    {
        gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Array of GValues");
    }
}
