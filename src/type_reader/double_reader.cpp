#include "type_reader/double_reader.h"

void DoubleTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys)
{
    GParamSpecDouble *pdouble = G_PARAM_SPEC_DOUBLE(pspec);

    keys[KEY_TYPE] = g_strdup("Double");

    keys[KEY_RANGE] = g_strdup_printf("%15.7g - %15.7g",
                                      pdouble->minimum, pdouble->maximum);

    keys[KEY_VALUE] = g_strdup_printf("%15.7g", g_value_get_double(value));
}