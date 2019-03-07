/**
 *  gst-inspector - A library for inspecting GStreamer's elements and plugins
 */

#include "gstdictionary.h"

#ifndef GST_INSPECTOR_H
#define GST_INSPECTOR_H

G_BEGIN_DECLS

/**
 *  @brief Inspection function for elements
 *
 *  @param element An element to inspect. The function should not take
 *  ownership on the received element and should not free it.
 *  @param result An unitialized GValue to fill with the inspection results.
 *  At the end of the inspction the GValue should either be initialized and 
 *  hold either a string, an array or a dictionary with the inspected data,
 *  or stay uninitialized if the inspection failed.
 */
typedef void (*GstElementInspectFunc)(GstElement *element, GValue *result);

/**
 *  @brief Inspection function for plugins
 *
 *  @param plugin A plugin to inspect. The function should not take ownership
 *  on the received plugin and should not free it.
 *  @param result An unitialized GValue to fill with the inspection results.
 *  At the end of the inspction the GValue should either be initialized and 
 *  hold either a string, an array or a dictionary with the inspected data,
 *  or stay uninitialized if the inspection failed.
 */
typedef void (*GstPluginInspectFunc)(GstPlugin *plugin, GValue *result);

// Element inspection
GST_INSPECTOR_API gboolean
gst_inspector_register_element_inspector(GstElementInspectFunc inspector,
                                         gchar *name,
                                         gchar *longname,
                                         gint position);
GST_INSPECTOR_API gboolean gst_inspector_remove_element_inspector(const gchar *name);
GST_INSPECTOR_API void gst_inspector_clear_element_inspectors(void);
GST_INSPECTOR_API gchar **gst_inspector_get_installed_element_inspectors(void);

// Plugin inspection
GST_INSPECTOR_API gboolean
gst_inspector_register_plugin_inspector(GstPluginInspectFunc inspector,
                                        gchar *name,
                                        gchar *longname,
                                        gint position);
GST_INSPECTOR_API gboolean gst_inspector_remove_plugin_inspector(const gchar *name);
GST_INSPECTOR_API void gst_inspector_clear_plugin_inspectors(void);
GST_INSPECTOR_API gchar **gst_inspector_get_installed_plugin_inspectors(void);

// Inspection functions
GST_INSPECTOR_API GstStructure *gst_inspector_inspect_element(GstElementFactory *factory);
GST_INSPECTOR_API GstStructure *gst_inspector_inspect_plugin(GstPlugin *plugin);
GST_INSPECTOR_API GstStructure *gst_inspector_inspect_plugin_feature(GstPluginFeature *feature);
GST_INSPECTOR_API GstStructure *gst_inspector_inspect_by_name(const gchar *object_name);

// Utilities
GST_INSPECTOR_API void gst_inspector_get_installed_plugins(GstPluginFlags flags,
                                                           const gchar *version,
                                                           GValue *list);
GST_INSPECTOR_API void gst_inspector_get_installed_features(GstPluginFlags flags,
                                                            const gchar *version,
                                                            GValue *list);

G_END_DECLS

#endif //GST_INSPECTOR_H
