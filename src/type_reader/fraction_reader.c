#include "type_reader/type_reader_priv.h"

/**
 *  @addtogroup type-readers
 *  @subsection fraction-reader Fraction type reader
 *  The fraction type reader (for GST_TYPE_FRACTION) parses the following 
 *  additional fields:
 *  - <b>Type</b> - Property type (Fraction)
 *  - <b>Range</b> - Range of possible values for the property
 *  - <b>Default Value</b> - Property's default value
 */
void gst_fraction_type_reader_fill_type(GParamSpec *pspec,
                                        GValue *value,
                                        GstStructure *dictionary)
{
    GstParamSpecFraction *pspec_fraction = NULL;

    g_return_if_fail(GST_VALUE_HOLDS_FRACTION(value));
    g_return_if_fail(GST_IS_PARAM_SPEC_FRACTION(pspec));

    pspec_fraction = GST_PARAM_SPEC_FRACTION(pspec);

    gst_dictionary_set_static_string(dictionary, KEY_TYPE, "Fraction");

    gst_dictionary_set_string(dictionary, KEY_RANGE,
                              g_strdup_printf("%d/%d - %d/%d",
                                              pspec_fraction->min_num,
                                              pspec_fraction->min_den,
                                              pspec_fraction->max_num,
                                              pspec_fraction->max_den));

    gst_dictionary_set_string(dictionary, KEY_VALUE,
                              g_strdup_printf("%d/%d",
                                              gst_value_get_fraction_numerator(value),
                                              gst_value_get_fraction_denominator(value)));
}
