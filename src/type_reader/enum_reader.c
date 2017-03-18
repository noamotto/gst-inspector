#include "type_reader/enum_reader.h"

static gchar *gst_enum_type_reader_find_default(
    const GEnumValue *values,
    const GValue * value)
{
    gint enum_value;
    enum_value = g_value_get_enum(value);
    const gchar *value_nick = "";

    for (guint j = 0; NULL != values[j].value_name; j++)
    {
        if (values[j].value == enum_value)
        {
            value_nick = values[j].value_nick;
            break;
        }
    }

    return g_strdup_printf("%d, \"%s\"", enum_value, value_nick);
}

static gchar *gst_enum_type_reader_parse_options(
    const GEnumValue *values)
{
    GString *options_string = g_string_new(NULL);

    for (guint j = 0; NULL != values[j].value_name; j++)
    {
        gchar *option = g_strdup_printf("(%d): %-16s - %s\n",
                                        values[j].value,
                                        values[j].value_nick,
                                        values[j].value_name);

        g_string_append(options_string, option);
        g_free(option);
    }

    return g_string_free(options_string, FALSE);
}

void gst_enum_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;
    const GEnumValue *values;

    g_return_if_fail(G_IS_PARAM_SPEC_ENUM(pspec));
    g_return_if_fail(G_VALUE_HOLDS_ENUM(value));

    values = G_ENUM_CLASS(g_type_class_peek(pspec->value_type))->values;

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value,
                        g_strdup_printf("Enum \"%s\"",
                                        g_type_name(G_VALUE_TYPE(value))));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, gst_enum_type_reader_find_default(values, value));
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, gst_enum_type_reader_parse_options(values));
    gst_structure_take_value(dictionary, KEY_OPTIONS, &key_value);
}

GST_DEFINE_TYPE_READER_TYPE(GstEnumTypeReader, gst_enum_type_reader,
                            gst_enum_type_reader_fill_type, NULL, NULL, NULL);
