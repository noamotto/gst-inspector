#include "type_reader/uint_reader.h"

void gst_uint_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;
    GParamSpecUInt *pspec_uint = NULL;

    g_return_if_fail(G_VALUE_HOLDS_UINT(value));
    g_return_if_fail(G_IS_PARAM_SPEC_UINT(pspec));

    pspec_uint = G_PARAM_SPEC_UINT(pspec);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup("Unsigned Integer"));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value,
                        g_strdup_printf("%u - %u", pspec_uint->minimum,
                                        pspec_uint->maximum));
    gst_structure_take_value(dictionary, KEY_RANGE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("%u",
                                                    g_value_get_uint(value)));
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}

GST_DEFINE_TYPE_READER_TYPE(GstUIntTypeReader, gst_uint_type_reader,
                            gst_uint_type_reader_fill_type, NULL, NULL, NULL);
