/**
 *  gst-inspector private utilities header
 */

#ifndef GST_INSPECTOR_PRIV_H
#define GST_INSPECTOR_PRIV_H

#include "gstinspector.h"

G_BEGIN_DECLS

#define G_VALUE_ARRAY_NEW(a)                                              \
    a = g_array_new(FALSE, FALSE, sizeof(GValue)); \
    g_array_set_clear_func(a, (GDestroyNotify)g_value_unset);

GArray *parse_caps(const GstCaps *caps);
GstStructure *parse_object_property(GObject *object, GParamSpec *pspec);
gchar *get_rank_name(gint rank);
GArray *parse_type_hierarchy(GType type);
GArray *parse_type_interfaces(GType type);
gboolean gtype_needs_ptr_marker(GType type);
GstStructure *parse_signal(GSignalQuery *query);

G_END_DECLS

#endif //GST_INSPECTOR_PRIV_H
