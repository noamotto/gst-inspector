#include "type_reader/type_reader_priv.h"

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
        gst_dictionary_set_array(dictionary, KEY_VALUE, parse_caps(caps));
    }
}
