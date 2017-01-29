#include "include/type_reader/long_reader.h"

void LongTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys)
{
    GParamSpecLong *plong = G_PARAM_SPEC_LONG(pspec);

    keys[KEY_TYPE] = g_strdup("Long");

    keys[KEY_RANGE] = g_strdup_printf("%ld - %ld",
                                      plong->minimum, plong->maximum);

    keys[KEY_VALUE] = g_strdup_printf("%ld", g_value_get_long(value));
}