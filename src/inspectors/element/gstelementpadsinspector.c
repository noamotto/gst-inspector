#include "gstinspectors.h"
#include "gstinspector_priv.h"

static GstStructure *parse_pad(GstPad *pad)
{
    GstCaps *caps;
    GstStructure *pad_dict = gst_structure_new_empty("pad");

    gst_dictionary_set_string(pad_dict, KEY_NAME, gst_pad_get_name(pad));

    if (gst_pad_get_direction(pad) == GST_PAD_SRC)
    {
        gst_dictionary_set_static_string(pad_dict, KEY_TYPE, "SRC");
    }
    else if (gst_pad_get_direction(pad) == GST_PAD_SINK)
    {
        gst_dictionary_set_static_string(pad_dict, KEY_TYPE, "SINK");
    }
    else
    {
        gst_dictionary_set_static_string(pad_dict, KEY_TYPE, "UNKNOWN");
    }

    if (pad->padtemplate)
    {
        gst_dictionary_set_static_string(pad_dict, "Pad Template", pad->padtemplate->name_template);
    }

    caps = gst_pad_get_current_caps(pad);
    if (caps)
    {
        GValue caps_arr = G_VALUE_INIT;
        parse_caps(caps, &caps_arr);
        gst_dictionary_set_array(pad_dict, "Capabilities", &caps_arr);

        gst_caps_unref(caps);
    }

    return pad_dict;
}

void gst_inspector_inspect_element_pads(GstElement *element, GValue *result)
{
    g_return_if_fail(GST_IS_ELEMENT(element));

    if (!element->numpads)
    {
        g_value_init(result, G_TYPE_STRING);
        g_value_set_static_string(result, "none");
    }
    else
    {
        const GList *pads = element->pads;

        g_value_init(result, GST_TYPE_LIST);

        while (pads)
        {
            gst_array_append_subdictionary(result, parse_pad(GST_PAD(pads->data)));
            pads = g_list_next(pads);
        }
    }
}
