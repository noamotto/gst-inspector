/**
 *  @file gstinspectors.h
 *  
 *  @brief Header for all the stock inspectors. These can be used
 *  to re-register removed plugins, for custom inspection results
 */

#include "gstinspector.h"

#ifndef GST_INSPECTORS_H
#define GST_INSPECTORS_H

G_BEGIN_DECLS

// Element inspectors
GST_INSPECTOR_API void gst_inspector_inspect_factory_details(GstElement *element, GValue *result);
GST_INSPECTOR_API void gst_inspector_inspect_element_plugin(GstElement *element, GValue *result);
GST_INSPECTOR_API void gst_inspector_inspect_element_hierarchy(GstElement *element, GValue *result);
GST_INSPECTOR_API void gst_inspector_inspect_element_interfaces(GstElement *element, GValue *result);
GST_INSPECTOR_API void gst_inspector_inspect_pad_templates(GstElement *element, GValue *result);
GST_INSPECTOR_API void gst_inspector_inspect_element_clocking(GstElement *element, GValue *result);
GST_INSPECTOR_API void gst_inspector_inspect_element_uri_handler(GstElement *element, GValue *result);
GST_INSPECTOR_API void gst_inspector_inspect_element_pads(GstElement *element, GValue *result);
GST_INSPECTOR_API void gst_inspector_inspect_element_properties(GstElement *element, GValue *result);
GST_INSPECTOR_API void gst_inspector_inspect_element_signals(GstElement *element, GValue *result);
GST_INSPECTOR_API void gst_inspector_inspect_element_actions(GstElement *element, GValue *result);

// Plugin inspectors
GST_INSPECTOR_API void gst_inspector_inspect_plugin_details(GstPlugin *plugin, GValue *result);

G_END_DECLS

#endif // GST_INSPECTORS_H
