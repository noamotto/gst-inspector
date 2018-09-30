#include "gstinspectors.h"
#include "gstinspector_priv.h"

GstStructure *gst_inspector_inspect_element_plugin(GstElement *element)
{
    GstStructure *dictionary = NULL;
    GstElementFactory *factory;
    GstPlugin *plugin;

    g_return_val_if_fail(GST_IS_ELEMENT(element), NULL);

    factory = gst_element_get_factory(element);
    plugin = gst_plugin_feature_get_plugin(GST_PLUGIN_FEATURE(factory));
    if (plugin)
    {
        dictionary = gst_inspector_inspect_plugin_details(plugin);
        gst_object_unref(plugin);
    }

    return dictionary;
}
