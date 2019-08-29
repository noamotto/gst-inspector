/**
 *  @file gstelementurihandlerinspector.c
 *  @brief Element URI handler inspector implementation
 */
#include "gstinspectors.h"
#include "gstinspector_priv.h"

static void parse_uri_type(GstElement *element, GstStructure *uri_data)
{
    GstURIType type = gst_uri_handler_get_uri_type(GST_URI_HANDLER(element));

    if (type == GST_URI_SRC)
    {
        gst_dictionary_set_static_string(uri_data, "URI handler type", "source");
    }
    else if (type == GST_URI_SINK)
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
        GValue protocols_list = G_VALUE_INIT;
        g_value_init(&protocols_list, GST_TYPE_LIST);

        for (; *uri_protocols != NULL; uri_protocols++)
        {
            gst_array_append_static_string(&protocols_list, *uri_protocols);
        }
        gst_dictionary_set_array(uri_data, "Supported URI protocols", &protocols_list);
    }
    else
    {
        gst_dictionary_set_static_string(uri_data, "Supported URI protocols", "none");
    }
}

/**
 *  @brief Inspects URI handling capabilities
 * 
 *  Inspects the URI handling capabilities of a single element
 * 
 *  @param element Element to inspect
 *  @param result 
 *  @parblock
 *  The inspected data
 * 
 *  If the element implements GstUriHandler, then the inspected data consists of
 *      these fields:
 *  - <b>URI handler type</b> - The URI handler type (source/sink)
 *  - <b>Supported URI protocols</b> - Array of names of supported URI protocols,
 *      if found. Otherwise the string "none"
 * 
 *  Otherwise, the inspected data is a string informing that the element has no 
 *      URI capabilities.
 *  @endparblock
 */
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
        g_value_take_boxed(result, uri_data);
    }
    else
    {
        g_value_init(result, G_TYPE_STRING);
        g_value_set_static_string(result, "Element has no URI handling capabilities.");
    }
}
