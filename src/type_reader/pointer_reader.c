#include "type_reader/pointer_reader.h"

void gst_pointer_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;

    (void)value;
    g_value_init(&key_value, G_TYPE_STRING);

    if (pspec->value_type != G_TYPE_POINTER)
    {
        g_value_take_string(&key_value, g_strdup_printf("Pointer of type \"%s\".",
                                                        g_type_name(pspec->value_type)));
        gst_structure_take_value(dictionary, KEY_TYPE, &key_value);
    }
    else
    {
        g_value_take_string(&key_value, g_strdup_printf("Pointer."));
        gst_structure_take_value(dictionary, KEY_TYPE, &key_value);
    }
}

GST_DEFINE_TYPE_READER_TYPE(GstPointerTypeReader, gst_pointer_type_reader,
                            gst_pointer_type_reader_fill_type, NULL, NULL, NULL);
