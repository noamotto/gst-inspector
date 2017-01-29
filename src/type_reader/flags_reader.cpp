#include "include/type_reader/flags_reader.h"

gchar *FlagsTypeReader::find_default(
    const GFlagsClass *flags_class,
    GValue *const value)
{
    GFlagsValue *values = flags_class->values;
    GString *flags_string = NULL;
    guint flags_left = g_value_get_flags(value);

    /* first look for an exact match and count the number of values */
    for (guint i = 0; i < flags_class->n_values; ++i)
    {
        if (values[i].value == flags_left)
        {
            return g_strdup(values[i].value_nick);
        }
    }

    flags_string = g_string_new(NULL);

    /* we assume the values are sorted from lowest to highest value */
    for (guint i = flags_class->n_values - 1; i > 0 && NULL != flags_left; --i)
    {
        if (values[i].value != 0 && (flags_left & values[i].value) == values[i].value)
        {
            if (flags_string->len > 0)
            {
                g_string_append_c(flags_string, '+');
            }
            g_string_append(flags_string, values[i].value_nick);
            flags_left -= values[i].value;
        }
    }

    if (flags_string->len == 0)
        g_string_assign(flags_string, "(none)");

    return g_string_free(flags_string, FALSE);
}

void FlagsTypeReader::populate_options(
    const GFlagsClass *flags_class,
    param_keys &keys)
{
    GFlagsValue *values = flags_class->values;
    GString *options_string = g_string_new(NULL);

    for (guint i = 0; i < flags_class->n_values; i++)
    {
        gchar *option = g_strdup_printf("(0x%08x): %-16s - %s\n", "",
                                        values[i].value,
                                        values[i].value_nick,
                                        values[i].value_name);
        options_string = g_string_append(options_string, option);

        g_free(option);
    }

    keys[KEY_OPTIONS] = g_string_free(options_string, FALSE);
}

void FlagsTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys)
{
    GParamSpecFlags *pflags = G_PARAM_SPEC_FLAGS(pspec);
    const GFlagsValue *values;
    gchar *value_string;

    values = pflags->flags_class->values;

    value_string = find_default(pflags->flags_class, value);
    keys[KEY_TYPE] = g_strdup_printf("flags_left \"%s\"", G_VALUE_TYPE_NAME(value));
    keys[KEY_VALUE] = g_strdup_printf("0x%08x, \"%s\"",
                                      g_value_get_flags(value), value_string);

    populate_options(pflags->flags_class, keys);
    g_free(value_string);
}