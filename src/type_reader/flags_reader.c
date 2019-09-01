#include "type_reader/type_reader_priv.h"

static gchar *gst_flags_type_reader_find_default(const GFlagsClass *flags_class,
                                                 const GValue *value)
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
    for (gint i = (gint)(flags_class->n_values - 1); i >= 0 && 0 != flags_left; --i)
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
    {
        g_string_assign(flags_string, "(none)");
    }

    return g_string_free(flags_string, FALSE);
}

static void gst_flags_type_reader_parse_options(const GFlagsClass *flags_class,
                                                GValue *result)
{
    GFlagsValue *values = flags_class->values;
    g_value_init(result, GST_TYPE_LIST);

    for (guint i = 0; i < flags_class->n_values; i++)
    {
        gst_array_append_string(result,
                                g_strdup_printf("(0x%08x): %-16s - %s",
                                                values[i].value,
                                                values[i].value_nick,
                                                values[i].value_name));
    }
}

/**
 *  @addtogroup type-readers
 *  @subsection flags-reader Flags type reader
 *  The flags type reader (for G_TYPE_FLAGS and subtypes) parses the following 
 *  additional fields:
 *  - <b>Type</b> - Property type. Includes the specific flags type
 *  - <b>Default Value</b> - Property's default value, as a hexadecimal 
 *      representation of the value followed by the flags it turns on, or "(none)"
 *      if no flag is turned on.
 *  - <b>Options</b> - Flag options. An array of strings, each string contains one
 *      flag option as value followed by the option's name and description.
 */
void gst_flags_type_reader_fill_type(GParamSpec *pspec,
                                     GValue *value,
                                     GstStructure *dictionary)
{
    GParamSpecFlags *pspec_flags = NULL;
    gchar *value_string = NULL;
    GValue options = G_VALUE_INIT;

    g_return_if_fail(G_IS_PARAM_SPEC_FLAGS(pspec));
    g_return_if_fail(G_VALUE_HOLDS_FLAGS(value));

    pspec_flags = G_PARAM_SPEC_FLAGS(pspec);

    value_string =
        gst_flags_type_reader_find_default(pspec_flags->flags_class, value);

    gst_dictionary_set_string(dictionary, KEY_TYPE,
                              g_strdup_printf("Flags \"%s\"",
                                              G_VALUE_TYPE_NAME(value)));

    gst_dictionary_set_string(dictionary, KEY_VALUE,
                              g_strdup_printf("0x%08x, \"%s\"",
                                              g_value_get_flags(value),
                                              value_string));

    gst_flags_type_reader_parse_options(pspec_flags->flags_class, &options);
    gst_dictionary_set_array(dictionary, KEY_OPTIONS, &options);

    g_free(value_string);
}
