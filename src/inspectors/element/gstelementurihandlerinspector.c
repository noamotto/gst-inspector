#include "gstinspectors.h"
#include "gstinspector_priv.h"

static void parse_uri_type(GstElement *element, GstStructure *uri_data)
{
    if (gst_uri_handler_get_uri_type(GST_URI_HANDLER(element)) == GST_URI_SRC)
    {
        gst_dictionary_set_static_string(uri_data, "URI handler type", "source");
    }
    else if (gst_uri_handler_get_uri_type(GST_URI_HANDLER(element)) == GST_URI_SINK)
    {
        gst_dictionary_set_static_string(uri_data, "URI handler type", "sink");
    }
    else
    {
        gst_dictionary_set_static_string(uri_data, "URI handler type", "unknown");
    }
}

static void parse_uri_protocols(GstElement *element, GstStructure *uri_data)
{
    const gchar *const *uri_protocols =
        gst_uri_handler_get_protocols(GST_URI_HANDLER(element));

    if (uri_protocols && *uri_protocols)
    {
        GArray *protocols_list = g_array_new(FALSE, FALSE, sizeof(GValue));
        g_array_set_clear_func(protocols_list, (GDestroyNotify)g_value_unset);

        for (; *uri_protocols != NULL; uri_protocols++)
        {
            g_array_add_static_string(protocols_list, *uri_protocols);
        }
        gst_dictionary_set_array(uri_data, "Supported URI protocols", protocols_list);
    }
    else
    {
        gst_dictionary_set_static_string(uri_data, "Supported URI protocols", "none");
    }
}

void gst_inspector_inspect_element_uri_handler(GstElement *element, GValue *result)
{
    g_return_if_fail(GST_IS_ELEMENT(element));

    if (GST_IS_URI_HANDLER(element))
    {
        GstStructure *uri_data;

        g_value_init(result, GST_TYPE_STRUCTURE);
        uri_data = gst_structure_new_empty("urihandler");

        parse_uri_type(element, uri_data);
        parse_uri_protocols(element, uri_data);
    }
    else
    {
        g_value_init(result, G_TYPE_STRING);
        g_value_set_static_string(result, "Element has no URI handling capabilities.");
    }
}
