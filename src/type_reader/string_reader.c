#include "type_reader/string_reader.h"

void gst_string_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
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

GST_DEFINE_TYPE_READER_TYPE(GstStringTypeReader, gst_string_type_reader,
                            gst_string_type_reader_fill_type, NULL, NULL, NULL);
