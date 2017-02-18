#include "type_reader/int64_reader.h"

void Int64TypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys) const
{
    GParamSpecInt64 *pint64 = G_PARAM_SPEC_INT64(pspec);

    keys[KEY_TYPE] = g_strdup("Integer64");

    keys[KEY_RANGE] = g_strdup_printf("%" G_GINT64_FORMAT " - %" G_GINT64_FORMAT,
                                      pint64->minimum, pint64->maximum);

    keys[KEY_VALUE] = g_strdup_printf("%" G_GINT64_FORMAT, g_value_get_int64(value));
}