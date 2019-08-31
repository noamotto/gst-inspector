/**
 *  Header with all the stock inspectors, for removing and re-registering.
 */

#include "gstinspector.h"

#ifndef GST_INSPECTORS_H
#define GST_INSPECTORS_H

G_BEGIN_DECLS

// Element inspectors
#define GST_INSPECTOR_FACTORY_DETAILS_NAME ("factorydetails")
#define GST_INSPECTOR_FACTORY_DETAILS_LONGNAME ("Factory Details")
GST_INSPECTOR_API void gst_inspector_inspect_factory_details(GstElement *element, GValue *result);

#define GST_INSPECTOR_PLUGIN_NAME ("elementplugin")
#define GST_INSPECTOR_PLUGIN_LONGNAME ("Plugin Details")
GST_INSPECTOR_API void gst_inspector_inspect_element_plugin(GstElement *element, GValue *result);

#define GST_INSPECTOR_HIERARCHY_NAME ("hierarchy")
#define GST_INSPECTOR_HIERARCHY_LONGNAME ("Type Hierarchy")
GST_INSPECTOR_API void gst_inspector_inspect_element_hierarchy(GstElement *element, GValue *result);

#define GST_INSPECTOR_INTERFACES_NAME ("interfaces")
#define GST_INSPECTOR_INTERFACES_LONGNAME ("Implemented Interfaces")
GST_INSPECTOR_API void gst_inspector_inspect_element_interfaces(GstElement *element, GValue *result);

#define GST_INSPECTOR_PAD_TEMPLATES_NAME ("padtemplates")
#define GST_INSPECTOR_PAD_TEMPLATES_LONGNAME ("Pad Templates")
GST_INSPECTOR_API void gst_inspector_inspect_pad_templates(GstElement *element, GValue *result);

#define GST_INSPECTOR_CLOCKING_NAME ("clocking")
#define GST_INSPECTOR_CLOCKING_LONGNAME ("Clocking Interaction")
GST_INSPECTOR_API void gst_inspector_inspect_element_clocking(GstElement *element, GValue *result);

#define GST_INSPECTOR_URI_HANDLER_NAME ("urihandler")
#define GST_INSPECTOR_URI_HANDLER_LONGNAME ("URI handling capabilities")
GST_INSPECTOR_API void gst_inspector_inspect_element_uri_handler(GstElement *element, GValue *result);

#define GST_INSPECTOR_PADS_NAME ("pads")
#define GST_INSPECTOR_PADS_LONGNAME ("Pads")
GST_INSPECTOR_API void gst_inspector_inspect_element_pads(GstElement *element, GValue *result);

#define GST_INSPECTOR_PROPERTIES_NAME ("params")
#define GST_INSPECTOR_PROPERTIES_LONGNAME ("Element Properties")
GST_INSPECTOR_API void gst_inspector_inspect_element_properties(GstElement *element, GValue *result);

#define GST_INSPECTOR_SIGNALS_NAME ("signals")
#define GST_INSPECTOR_SIGNALS_LONGNAME ("Element Signals")
GST_INSPECTOR_API void gst_inspector_inspect_element_signals(GstElement *element, GValue *result);

#define GST_INSPECTOR_ACTIONS_NAME ("actions")
#define GST_INSPECTOR_ACTIONS_LONGNAME ("Element Actions")
GST_INSPECTOR_API void gst_inspector_inspect_element_actions(GstElement *element, GValue *result);

#define GST_INSPECTOR_CHILDREN_NAME ("children")
#define GST_INSPECTOR_CHILDREN_LONGNAME ("Children")
GST_INSPECTOR_API void gst_inspector_inspect_element_children(GstElement *element, GValue *result);

#define GST_INSPECTOR_PRESETS_NAME ("presets")
#define GST_INSPECTOR_PRESETS_LONGNAME ("Presets")
GST_INSPECTOR_API void gst_inspector_inspect_element_presets(GstElement *element, GValue *result);

// Plugin inspectors
#define GST_INSPECTOR_PLUGIN_DETAILS_NAME ("plugindetails")
#define GST_INSPECTOR_PLUGIN_DETAILS_LONGNAME ("Plugin Details")
GST_INSPECTOR_API void gst_inspector_inspect_plugin_details(GstPlugin *plugin, GValue *result);

#define GST_INSPECTOR_PLUGIN_FEATURES_NAME ("pluginfeatures")
#define GST_INSPECTOR_PLUGIN_FEATURES_LONGNAME ("Plugin Features")
GST_INSPECTOR_API void gst_inspector_inspect_plugin_features(GstPlugin *plugin, GValue *result);

G_END_DECLS

#endif // GST_INSPECTORS_H
