#include "type_reader/type_reader_priv.h"

/**
 *  @addtogroup type-readers
 *  @subsection long-reader Long type reader
 *  The long type reader (for G_TYPE_LONG) parses the following 
 *  additional fields:
 *  - <b>Type</b> - Property type (Long)
 *  - <b>Range</b> - Range of possible values for the property
 *  - <b>Default Value</b> - Property's default value
 */
void gst_long_type_reader_fill_type(GParamSpec *pspec,
                                    GValue *value,
                                    GstStructure *dictionary)
{
    GParamSpecLong *pspec_long = NULL;

    g_return_if_fail(G_VALUE_HOLDS_LONG(value));
    g_return_if_fail(G_IS_PARAM_SPEC_LONG(pspec));

    pspec_long = G_PARAM_SPEC_LONG(pspec);

    gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Long");

    gst_dictionary_set_string(dictionary, KEY_RANGE,
                              g_strdup_printf("%ld - %ld",
                                              pspec_long->minimum,
                                              pspec_long->maximum));

    gst_dictionary_set_string(dictionary, KEY_VALUE,
                              g_strdup_printf("%ld", g_value_get_long(value)));
}
