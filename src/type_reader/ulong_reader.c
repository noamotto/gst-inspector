#include "type_reader/ulong_reader.h"

void gst_ulong_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;
    GParamSpecULong *pspec_ulong = NULL;

    g_return_if_fail(G_VALUE_HOLDS_ULONG(value));
    g_return_if_fail(G_IS_PARAM_SPEC_ULONG(pspec));

    pspec_ulong = G_PARAM_SPEC_ULONG(pspec);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup("Unsigned Long"));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value,
                        g_strdup_printf("%lu - %lu", pspec_ulong->minimum,
                                        pspec_ulong->maximum));
    gst_structure_take_value(dictionary, KEY_RANGE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("%lu",
                                                    g_value_get_ulong(value)));
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}

GST_DEFINE_TYPE_READER_TYPE(GstULongTypeReader, gst_ulong_type_reader,
                            gst_ulong_type_reader_fill_type, NULL, NULL, NULL);
