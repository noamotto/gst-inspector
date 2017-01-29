#include "include/type_reader/string_reader.h"

void StringTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys)
{
    const char *string_val = g_value_get_string(value);

    keys[KEY_TYPE] = g_strdup("String");

    if (string_val == NULL)
    {
        keys[KEY_VALUE] = g_strdup("null");
    }
    else
    {
        keys[KEY_VALUE] = g_strdup(string_val);
    }
}