#include "type_reader/bool_reader.h"

void gst_bool_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;
    gboolean bool_val;

    g_return_if_fail(G_VALUE_HOLDS_BOOLEAN(value));

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup("Boolean"));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    bool_val = g_value_get_boolean(value);
    g_value_take_string(&key_value, g_strdup(bool_val ? "true" : "false"));
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}

GST_DEFINE_TYPE_READER_TYPE(GstBoolTypeReader, gst_bool_type_reader,
   gst_bool_type_reader_fill_type, NULL, NULL, NULL);