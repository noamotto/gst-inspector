#include "type_reader/object_reader.h"

void gst_object_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;

    (void)value;
    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("Object of type \"%s\"",
                                                    g_type_name(pspec->value_type)));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);
}

GST_DEFINE_TYPE_READER_TYPE(GstObjectTypeReader, gst_object_type_reader,
                            gst_object_type_reader_fill_type, NULL, NULL, NULL);
