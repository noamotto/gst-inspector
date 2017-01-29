#include "include/type_reader/float_reader.h"

void FloatTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys)
{
        GParamSpecFloat *pfloat = G_PARAM_SPEC_FLOAT(pspec);

        keys[KEY_TYPE] = g_strdup("Float");

        keys[KEY_RANGE] = g_strdup_printf("%15.7g - %15.7g",
                                                  pfloat->minimum, pfloat->maximum);

        keys[KEY_VALUE] = g_strdup_printf("%15.7g", g_value_get_float(value));
}