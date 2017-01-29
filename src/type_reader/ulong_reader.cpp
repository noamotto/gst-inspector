#include "include/type_reader/ulong_reader.h"

void ULongTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys)
{
        GParamSpecULong *pulong = G_PARAM_SPEC_ULONG(pspec);

        keys[KEY_TYPE] = g_strdup("Unsigned Long");

        keys[KEY_RANGE] = g_strdup_printf("%lu - %lu",
                                                  pulong->minimum, pulong->maximum);

        keys[KEY_VALUE] = g_strdup_printf("%lu", g_value_get_ulong(value));
}