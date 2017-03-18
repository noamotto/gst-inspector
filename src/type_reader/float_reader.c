#include "type_reader/float_reader.h"

void gst_float_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    gfloat float_val = 0;
    GValue key_value = G_VALUE_INIT;
    GParamSpecFloat *pspec_float = NULL;

    g_return_if_fail(G_VALUE_HOLDS_FLOAT(value));
    g_return_if_fail(G_IS_PARAM_SPEC_FLOAT(pspec));

    pspec_float = G_PARAM_SPEC_FLOAT(pspec);
    float_val = g_value_get_float(value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup("Float"));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value,
                        g_strdup_printf("%15.7g - %15.7g",
                                        pspec_float->minimum, pspec_float->maximum));
    gst_structure_take_value(dictionary, KEY_RANGE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("%15.7g",
                                                    g_value_get_float(value)));
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}

GST_DEFINE_TYPE_READER_TYPE(GstFloatTypeReader, gst_float_type_reader,
                            gst_float_type_reader_fill_type, NULL, NULL, NULL);
