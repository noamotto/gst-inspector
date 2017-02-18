#include "type_reader/enum_reader.h"

void EnumTypeReader::find_default(
    const GEnumValue *values,
    GValue *const value,
    param_keys &keys) const
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

    keys[KEY_VALUE] = g_strdup_printf("%d, \"%s\"", enum_value, value_nick);
}

void EnumTypeReader::populate_options(
    const GEnumValue *values,
    param_keys &keys) const
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

    keys[KEY_OPTIONS] = g_string_free(options_string, FALSE);
}

void EnumTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys) const
{
    const GEnumValue *values;

    values = G_ENUM_CLASS(g_type_class_peek(pspec->value_type))->values;

    keys[KEY_TYPE] = g_strdup_printf("Enum \"%s\"",
                                     g_type_name(G_VALUE_TYPE(value)));
                                
    find_default(values, value, keys);
    populate_options(values, keys);    
}