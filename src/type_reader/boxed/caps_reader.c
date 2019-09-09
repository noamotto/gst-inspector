#include "type_reader/type_reader_priv.h"

/**
 *  @addtogroup boxed-readers
 *  @subsection caps-reader Caps boxed content reader
 *  The caps boxed content reader (for GST_TYPE_CAPS) parses the following 
 *  additional fields:
 *  - <b>Default Value</b> - Property's default value. It can hold one of the 
 *  following:
 *      - If no caps were found, or NULL caps were found, it will hold the string
 *          "Caps (NULL)"
 *      - If ANY caps were found, it will hold the string "ANY"
 *      - If EMPTY caps were found, it will hold the string "EMPTY"
 *      - Otherwise, caps will be parsed as the Capabilities field in 
 *          gst_inspector_inspect_pad_templates()
 */
void gst_caps_content_reader_parse(GValue *boxed,
                                   GstStructure *dictionary)
{
    const GstCaps *caps;

    g_return_if_fail(GST_VALUE_HOLDS_CAPS(boxed));

    caps = gst_value_get_caps(boxed);
    if (!caps)
    {
        gst_dictionary_set_static_string(dictionary, KEY_VALUE, "Caps (NULL)");
    }
    else if (gst_caps_is_any(caps))
    {
        gst_dictionary_set_static_string(dictionary, KEY_VALUE, "ANY");
    }
    else if (gst_caps_is_empty(caps))
    {
        gst_dictionary_set_static_string(dictionary, KEY_VALUE, "EMPTY");
    }
    else
    {
        GValue caps_arr = G_VALUE_INIT;
        parse_caps(caps, &caps_arr);

        gst_dictionary_set_array(dictionary, KEY_VALUE, &caps_arr);
    }
}
