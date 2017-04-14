#include "type_reader/type_reader_priv.h"

void gst_string_type_reader_fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    GstStructure *const dictionary)
{
    const char *string_val = g_value_get_string(value);
    GValue key_value = G_VALUE_INIT;

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup("String"));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    if (string_val == NULL)
    {
        g_value_take_string(&key_value, g_strdup("null"));
        gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
    }
    else
    {
        g_value_take_string(&key_value, g_strdup(string_val));
        gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
    }
}
