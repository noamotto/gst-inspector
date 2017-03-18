#include "type_reader/fraction_reader.h"

void gst_fraction_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;
    GstParamSpecFraction *pspec_fraction = NULL;

    g_return_if_fail(GST_VALUE_HOLDS_FRACTION(value));
    g_return_if_fail(GST_IS_PARAM_SPEC_FRACTION(pspec));

    pspec_fraction = GST_PARAM_SPEC_FRACTION(pspec);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup("Fraction"));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value,
                        g_strdup_printf("%d/%d - %d/%d",
                                        pspec_fraction->min_num,
                                        pspec_fraction->min_den,
                                        pspec_fraction->max_num,
                                        pspec_fraction->max_den));

    gst_structure_take_value(dictionary, KEY_RANGE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value,
                        g_strdup_printf("%d/%d",
                                        gst_value_get_fraction_numerator(value),
                                        gst_value_get_fraction_denominator(value)));
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}

GST_DEFINE_TYPE_READER_TYPE(GstFractionTypeReader, gst_fraction_type_reader,
                            gst_fraction_type_reader_fill_type, NULL, NULL, NULL);
