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
 *  @param element An element factory to inspect. The function should not take
 *  ownership on the received element factory and should not free it.
 *
 *  @return A dictionary filled with the inspected data from the element, or
 *  NULL if no data was found.
 */
typedef GstStructure *(*GstElementInspectFunc)(GstElementFactory *element);

/**
 *  @brief Inspection function for plugins
 *
 *  @param plugin A plugin to inspect. The function should not take ownership
 *  on the received plugin and should not free it.
 *
 *  @return A dictionary filled with the inspected data from the plugin, or
 *  NULL if no data was found.
 */
typedef GstStructure *(*GstPluginInspectFunc)(GstPlugin *plugin);

void gst_inspector_init();

gboolean gst_inspector_register_element_inspector(GstElementInspectFunc *inspector,
                                                  const gchar *name,
                                                  guint location);
gboolean gst_inspector_remove_element_inspector(const gchar *name);
GList *gst_inspector_get_installed_element_inspectors();


gboolean gst_inspector_register_plugin_inspector(GstPluginInspectFunc *inspector,
                                                  const gchar *name,
                                                  guint location);
gboolean gst_inspector_remove_plugin_inspector(const gchar *name);
GList *gst_inspector_get_installed_plugin_inspectors();

// Inspection functions
GstStructure *gst_inspector_inspect_element(GstElementFactory *element);
GstStructure *gst_inspector_inspect_plugin(GstPlugin *plugin);
GstStructure *gst_inspector_inspect_by_name(const gchar *object_name);

G_END_DECLS

#endif //GST_INSPECTOR_H
