#include "type_reader/type_reader_priv.h"

/**
 *  @addtogroup type-readers
 *  @subsection ulong-reader Unsigned Long type reader
 *  The unsigned long type reader (for G_TYPE_ULONG) parses the following 
 *  additional fields:
 *  - <b>Type</b> - Property type (Unsigned Long)
 *  - <b>Range</b> - Range of possible values for the property
 *  - <b>Default Value</b> - Property's default value
 */
void gst_ulong_type_reader_fill_type(GParamSpec *pspec,
                                     GValue *value,
                                     GstStructure *dictionary)
{
    GParamSpecULong *pspec_ulong = NULL;

    g_return_if_fail(G_VALUE_HOLDS_ULONG(value));
    g_return_if_fail(G_IS_PARAM_SPEC_ULONG(pspec));

    pspec_ulong = G_PARAM_SPEC_ULONG(pspec);

    gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Unsigned Long");

    gst_dictionary_set_string(dictionary, KEY_RANGE, g_strdup_printf("%lu - %lu", pspec_ulong->minimum, pspec_ulong->maximum));

    gst_dictionary_set_string(dictionary, KEY_VALUE,
                              g_strdup_printf("%lu", g_value_get_ulong(value)));
}
