#include "type_reader/double_reader.h"

void gst_double_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    gdouble double_val = 0;
    GValue key_value = G_VALUE_INIT;
    GParamSpecDouble *pspec_double = NULL;

    g_return_if_fail(G_VALUE_HOLDS_DOUBLE(value));
    g_return_if_fail(G_IS_PARAM_SPEC_DOUBLE(pspec));

    pspec_double = G_PARAM_SPEC_DOUBLE(pspec);
    double_val = g_value_get_double(value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup("Double"));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value,
                        g_strdup_printf("%15.7g - %15.7g",
                                        pspec_double->minimum, pspec_double->maximum));
    gst_structure_take_value(dictionary, KEY_RANGE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("%15.7g",
                                                    g_value_get_double(value)));
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}

GST_DEFINE_TYPE_READER_TYPE(GstDoubleTypeReader, gst_double_type_reader,
                            gst_double_type_reader_fill_type, NULL, NULL, NULL);
