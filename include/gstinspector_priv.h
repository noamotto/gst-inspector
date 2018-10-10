/**
 *  gst-inspector private utilities header
 */

#ifndef GST_INSPECTOR_PRIV_H
#define GST_INSPECTOR_PRIV_H

#include "gstinspector.h"

G_BEGIN_DECLS

gboolean parse_field(GQuark field_id, const GValue *value, GValue *array);
void parse_caps(const GstCaps *caps, GValue *result);
GstStructure *parse_object_property(GObject *object, GParamSpec *pspec);
gchar *get_rank_name(gint rank);
void parse_type_hierarchy(GType type, GValue *result);
void parse_type_interfaces(GType type, GValue *result);
gboolean gtype_needs_ptr_marker(GType type);
GstStructure *parse_signal(GSignalQuery *query);

G_END_DECLS

#endif //GST_INSPECTOR_PRIV_H
