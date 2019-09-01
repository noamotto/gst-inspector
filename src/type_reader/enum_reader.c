#include "type_reader/type_reader_priv.h"

static gchar *gst_enum_type_reader_find_default(const GEnumValue *values,
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

static void gst_enum_type_reader_parse_options(const GEnumValue *values,
                                               GValue *result)
{
    g_value_init(result, GST_TYPE_LIST);

    for (guint j = 0; NULL != values[j].value_name; j++)
    {
        gst_array_append_string(result,
                                g_strdup_printf("(%d): %-16s - %s",
                                                values[j].value,
                                                values[j].value_nick,
                                                values[j].value_name));
    }
}

/**
 *  @addtogroup type-readers
 *  @subsection enum-reader Enum type reader
 *  The enum type reader (for G_TYPE_ENUM and subtypes) parses the following 
 *  additional fields:
 *  - <b>Type</b> - Property type. Includes the specific enum type
 *  - <b>Default Value</b> - Property's default value, as a numerc value followed
 *      by the string representation of the type.
 *  - <b>Options</b> - Enum options. An array of strings, each string contains one
 *      enum option as value followed by the option's name and description.
 */
void gst_enum_type_reader_fill_type(GParamSpec *pspec,
                                    GValue *value,
                                    GstStructure *dictionary)
{
    const GEnumValue *values;
    GValue options = G_VALUE_INIT;

    g_return_if_fail(G_IS_PARAM_SPEC_ENUM(pspec));
    g_return_if_fail(G_VALUE_HOLDS_ENUM(value));

    values = G_ENUM_CLASS(g_type_class_peek(pspec->value_type))->values;

    gst_dictionary_set_string(dictionary, KEY_TYPE,
                              g_strdup_printf("Enum \"%s\"",
                                              g_type_name(G_VALUE_TYPE(value))));

    gst_dictionary_set_string(dictionary, KEY_VALUE,
                              gst_enum_type_reader_find_default(values, value));

    gst_enum_type_reader_parse_options(values, &options);
    gst_dictionary_set_array(dictionary, KEY_OPTIONS, &options);
}
