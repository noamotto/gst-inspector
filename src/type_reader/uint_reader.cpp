#include "include/type_reader/uint_reader.h"

void UIntTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys)
{
        GParamSpecUInt *puint = G_PARAM_SPEC_UINT(pspec);

        keys[KEY_TYPE] = g_strdup("Unsigned Integer");

        keys[KEY_RANGE] = g_strdup_printf("%u - %u",
                                   puint->minimum, puint->maximum);

        keys[KEY_VALUE] = g_strdup_printf("%u", g_value_get_uint(value));
}