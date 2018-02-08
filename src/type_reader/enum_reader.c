#include "type_reader/type_reader_priv.h"

static gchar *gst_enum_type_reader_find_default(
    const GEnumValue *values,
    const GValue *value)
{
    gint enum_value;
    enum_value = g_value_get_enum(value);
    const gchar *value_nick = "unknown";

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

static GArray *gst_enum_type_reader_parse_options(
    const GEnumValue *values)
{
    GArray *options_array = g_array_new(FALSE, TRUE, sizeof(GValue));
    g_array_set_clear_func(options_array, (GDestroyNotify)g_value_unset);

    for (guint j = 0; NULL != values[j].value_name; j++)
    {
        GValue option_val = G_VALUE_INIT;
        gchar *option = g_strdup_printf("(%d): %-16s - %s",
                                        values[j].value,
                                        values[j].value_nick,
                                        values[j].value_name);

        g_value_init(&option_val, G_TYPE_STRING);
        g_value_take_string(&option_val, option);
        g_array_append_val(options_array, option_val);
    }

    return options_array;
}

void gst_enum_type_reader_fill_type(
    GParamSpec *pspec,
    GValue *value,
    GstStructure *dictionary)
{
    const GEnumValue *values;

    g_return_if_fail(G_IS_PARAM_SPEC_ENUM(pspec));
    g_return_if_fail(G_VALUE_HOLDS_ENUM(value));

    values = G_ENUM_CLASS(g_type_class_peek(pspec->value_type))->values;

    gst_dictionary_set_string(dictionary, KEY_TYPE,
                              g_strdup_printf("Enum \"%s\"",
                                              g_type_name(G_VALUE_TYPE(value))));

    gst_dictionary_set_string(dictionary, KEY_VALUE,
                              gst_enum_type_reader_find_default(values, value));

    gst_dictionary_set_array(dictionary, KEY_OPTIONS, gst_enum_type_reader_parse_options(values));
}
