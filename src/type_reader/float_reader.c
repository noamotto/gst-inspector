#include "type_reader/type_reader_priv.h"

/**
 *  @addtogroup type-readers
 *  @subsection float-reader Float type reader
 *  The float type reader (for G_TYPE_FLOAT) parses the following 
 *  additional fields:
 *  - <b>Type</b> - Property type (Float)
 *  - <b>Range</b> - Range of possible values for the property
 *  - <b>Default Value</b> - Property's default value
 */
void gst_float_type_reader_fill_type(GParamSpec *pspec,
                                     GValue *value,
                                     GstStructure *dictionary)
{
    gfloat float_val = 0;
    GParamSpecFloat *pspec_float = NULL;

    g_return_if_fail(G_VALUE_HOLDS_FLOAT(value));
    g_return_if_fail(G_IS_PARAM_SPEC_FLOAT(pspec));

    pspec_float = G_PARAM_SPEC_FLOAT(pspec);
    float_val = g_value_get_float(value);

    gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Float");

    gst_dictionary_set_string(dictionary, KEY_RANGE,
                              g_strdup_printf("%15.7g - %15.7g",
                                              pspec_float->minimum,
                                              pspec_float->maximum));

    gst_dictionary_set_string(dictionary, KEY_VALUE,
                              g_strdup_printf("%15.7g", g_value_get_float(value)));
}
