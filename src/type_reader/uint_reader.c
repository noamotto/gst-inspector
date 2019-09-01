#include "type_reader/type_reader_priv.h"

/**
 *  @addtogroup type-readers
 *  @subsection uint-reader Unsigned Integer type reader
 *  The unsigned integer type reader (for G_TYPE_UINT) parses the following 
 *  additional fields:
 *  - <b>Type</b> - Property type (Unsigned Integer)
 *  - <b>Range</b> - Range of possible values for the property
 *  - <b>Default Value</b> - Property's default value
 */
void gst_uint_type_reader_fill_type(GParamSpec *pspec,
                                    GValue *value,
                                    GstStructure *dictionary)
{
    GParamSpecUInt *pspec_uint = NULL;

    g_return_if_fail(G_VALUE_HOLDS_UINT(value));
    g_return_if_fail(G_IS_PARAM_SPEC_UINT(pspec));

    pspec_uint = G_PARAM_SPEC_UINT(pspec);

    gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Unsigned Integer");

    gst_dictionary_set_string(dictionary, KEY_RANGE,
                              g_strdup_printf("%u - %u",
                                              pspec_uint->minimum,
                                              pspec_uint->maximum));

    gst_dictionary_set_string(dictionary, KEY_VALUE,
                              g_strdup_printf("%u", g_value_get_uint(value)));
}
