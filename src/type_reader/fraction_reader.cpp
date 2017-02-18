#include "type_reader/fraction_reader.h"

void FractionTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys) const
{
    GstParamSpecFraction *pfraction = GST_PARAM_SPEC_FRACTION(pspec);

    keys[KEY_TYPE] = g_strdup("Fraction");

    keys[KEY_RANGE] = g_strdup_printf("%d/%d - %d/%d",
            pfraction->min_num, pfraction->min_den,
            pfraction->max_num, pfraction->max_den);
    keys[KEY_VALUE] = g_strdup_printf("%d/%d",
            gst_value_get_fraction_numerator(value),
            gst_value_get_fraction_denominator(value));
}