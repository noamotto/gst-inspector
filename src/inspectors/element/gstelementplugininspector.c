#include "gstinspectors.h"
#include "gstinspector_priv.h"

void gst_inspector_inspect_element_plugin(GstElement *element, GValue *result)
{
    GstElementFactory *factory;
    GstPlugin *plugin;

    g_return_if_fail(GST_IS_ELEMENT(element));

    factory = gst_element_get_factory(element);
    plugin = gst_plugin_feature_get_plugin(GST_PLUGIN_FEATURE(factory));
    if (plugin)
    {
        gst_inspector_inspect_plugin_details(plugin, result);
        gst_object_unref(plugin);
    }
}
