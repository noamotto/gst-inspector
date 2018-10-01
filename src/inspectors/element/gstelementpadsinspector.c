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
        gst_dictionary_set_array(pad_dict, "Capabilities", parse_caps(caps));

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
        const GList *pads;

        GArray *pads_array = g_array_new(FALSE, FALSE, sizeof(GValue));
        g_array_set_clear_func(pads_array, (GDestroyNotify)g_value_unset);
        g_value_init(result, G_TYPE_ARRAY);

        pads = element->pads;

        while (pads)
        {
            g_array_add_subdictionary(pads_array, parse_pad(GST_PAD(pads->data)));
            pads = g_list_next(pads);
        }

        g_value_take_boxed(result, pads_array);
    }
}
