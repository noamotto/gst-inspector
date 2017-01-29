#include "include/type_reader/int_reader.h"

void IntTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys)
{
    GParamSpecInt *pint = G_PARAM_SPEC_INT(pspec);

    keys[KEY_TYPE] = g_strdup("Integer");

    keys[KEY_RANGE] = g_strdup_printf("%d - %d",
                                      pint->minimum, pint->maximum);

    keys[KEY_VALUE] = g_strdup_printf("%d", g_value_get_int(value));
}