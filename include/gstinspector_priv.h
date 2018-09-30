/**
 *  gst-inspector private utilities header
 */

#ifndef GST_INSPECTOR_PRIV_H
#define GST_INSPECTOR_PRIV_H

#include "gstinspector.h"

G_BEGIN_DECLS

GArray *parse_caps(const GstCaps *caps);
GstStructure *parse_object_property(GObject *object, GParamSpec *pspec);
gchar *get_rank_name(gint rank);
GArray *parse_type_hierarchy(GType type);
GArray *parse_type_interfaces(GType type);

G_END_DECLS

#endif //GST_INSPECTOR_PRIV_H
