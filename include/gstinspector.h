/**
 *  gst-inspector - A library for inspecting GStreamer's elements and plugins
 */

#include "gstdictionary.h"

#ifndef GST_INSPECTOR_H
#define GST_INSPECTOR_H

G_BEGIN_DECLS

void gst_inspector_init();

GstStructure *gst_inspector_inspect_by_name(const gchar *object_name);

G_END_DECLS

#endif //GST_INSPECTOR_H
