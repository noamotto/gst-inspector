#include "type_reader/type_reader_priv.h"

/**
 *  @addtogroup type-readers
 *  @subsection string-reader String type reader
 *  The string type reader (for G_TYPE_STRING) parses the following 
 *  additional fields:
 *  - <b>Type</b> - Property type (String)
 *  - <b>Default Value</b> - Property's default value. "null" if default string
 *      is NULL.
 */
void gst_string_type_reader_fill_type(GParamSpec *pspec,
                                      GValue *value,
                                      GstStructure *dictionary)
{
    const char *string_val = NULL;

    g_return_if_fail(G_IS_PARAM_SPEC_STRING(pspec));
    g_return_if_fail(G_VALUE_HOLDS_STRING(value));

    gst_dictionary_set_static_string(dictionary, KEY_TYPE, "String");

    string_val = g_value_get_string(value);

    if (string_val == NULL)
    {
        gst_dictionary_set_static_string(dictionary, KEY_VALUE, "null");
    }
    else
    {
        gst_dictionary_set_string(dictionary, KEY_VALUE, g_strdup(string_val));
    }
}
