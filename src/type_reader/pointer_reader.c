#include "type_reader/type_reader_priv.h"

void gst_pointer_type_reader_fill_type(
    GParamSpec *pspec,
    GValue *value,
    GstStructure *dictionary)
{
    g_return_if_fail(G_IS_PARAM_SPEC_POINTER(pspec));
    g_return_if_fail(G_IS_VALUE(value));

    if (pspec->value_type != G_TYPE_POINTER)
    {
        gst_dictionary_set_string(dictionary, KEY_TYPE,
                                  g_strdup_printf("Pointer of type \"%s\".",
                                                  g_type_name(pspec->value_type)));
    }
    else
    {
        gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Pointer.");
    }
}
