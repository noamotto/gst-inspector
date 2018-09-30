/**
 *  @file gstinspectors.h
 *  
 *  @brief Header for all the stock inspectors
 */

#include "gstinspector.h"

#ifndef GST_INSPECTORS_H
#define GST_INSPECTORS_H

G_BEGIN_DECLS

// Element inspectors
GST_INSPECTOR_API GstStructure *gst_inspector_inspect_element_properties(GstElement *element);
GST_INSPECTOR_API GstStructure *gst_inspector_inspect_pad_templates(GstElement *element);
GST_INSPECTOR_API GstStructure *gst_inspector_inspect_factory_details(GstElement *element);

// Plugin inspectors
GST_INSPECTOR_API GstStructure *gst_inspector_inspect_plugin_details(GstPlugin *plugin);

G_END_DECLS

#endif // GST_INSPECTORS_H
