#include "include/type_reader/uint64_reader.h"

void UInt64TypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys)
{
    GParamSpecUInt64 *puint64 = G_PARAM_SPEC_UINT64(pspec);

    keys[KEY_TYPE] = g_strdup("Unsigned Integer64");

    keys[KEY_RANGE] = g_strdup_printf("%" G_GUINT64_FORMAT " - %" G_GUINT64_FORMAT,
                                      puint64->minimum, puint64->maximum);

    keys[KEY_VALUE] = g_strdup_printf("%" G_GUINT64_FORMAT, g_value_get_uint64(value));
}