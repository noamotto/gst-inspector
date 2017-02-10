#include "type_reader/bool_reader.h"

void BoolTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys)
{
        gboolean bool_val = g_value_get_boolean(value);

        keys[KEY_TYPE] = g_strdup("Boolean");

        keys[KEY_VALUE] = g_strdup(bool_val ? "true" : "false");
}