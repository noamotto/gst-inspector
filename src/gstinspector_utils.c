#include "gstinspector_priv.h"
#include "type_reader/type_reader.h"
#include <math.h>

#define KEY_FEATURES ("Features")

gboolean parse_field(GQuark field_id, const GValue *value, GValue *array)
{
    gchar *str = gst_value_serialize(value);
    gchar *field = g_strdup_printf("%s: %s",
                                   g_quark_to_string(field_id),
                                   str);

    gst_array_append_string(array, field);

    g_free(str);
    return TRUE;
}

void parse_caps(const GstCaps *caps, GValue *result)
{
    g_value_init(result, GST_TYPE_ARRAY);

    for (guint i = 0; i < gst_caps_get_size(caps); i++)
    {
        GstStructure *caps_dict = gst_structure_new_empty("caps_dict");
        GValue fields_array = G_VALUE_INIT;
        GstStructure *structure = gst_caps_get_structure(caps, i);
        GstCapsFeatures *features = gst_caps_get_features(caps, i);
        g_value_init(&fields_array, GST_TYPE_ARRAY);

        gst_dictionary_set_static_string(caps_dict, KEY_TYPE, gst_structure_get_name(structure));

        if (features && (gst_caps_features_is_any(features) ||
                         !gst_caps_features_is_equal(features,
                                                     GST_CAPS_FEATURES_MEMORY_SYSTEM_MEMORY)))
        {
            gst_dictionary_set_string(caps_dict, KEY_FEATURES,
                                      gst_caps_features_to_string(features));
        }
        gst_structure_foreach(structure,
                              (GstStructureForeachFunc)parse_field,
                              &fields_array);

        gst_dictionary_set_array(caps_dict, "Caps", &fields_array);

        gst_array_append_subdictionary(result, caps_dict);
    }
}

GstStructure *parse_object_property(GObject *object, GParamSpec *pspec)
{
    GstStructure *param_dict = gst_structure_new_empty(pspec->name);
    GValue param_val = G_VALUE_INIT;
    g_value_init(&param_val, pspec->value_type);

    if (pspec->flags & G_PARAM_READABLE && object)
    {
        g_object_get_property(object, pspec->name, &param_val);
    }
    else
    {
        g_param_value_set_default(pspec, &param_val);
    }

    gst_type_reader_fill_type(pspec, &param_val, param_dict);

    g_value_unset(&param_val);
    return param_dict;
}

gchar *get_rank_name(gint rank)
{
    static const gint ranks[4] = {
        GST_RANK_NONE, GST_RANK_MARGINAL, GST_RANK_SECONDARY, GST_RANK_PRIMARY};
    static const gchar *rank_names[4] = {"none", "marginal", "secondary",
                                         "primary"};

    gint best_match = 0;

    for (gint i = 0; i < 4; i++)
    {
        if (rank == ranks[i])
        {
            return g_strdup(rank_names[i]);
        }
        if (abs(rank - ranks[i]) < abs(rank - ranks[best_match]))
        {
            best_match = i;
        }
    }

    return g_strdup_printf("%s %c %d (%d)", rank_names[best_match],
                           (rank - ranks[best_match] > 0) ? '+' : '-',
                           abs(ranks[best_match] - rank), rank);
}

void parse_type_hierarchy(GType type, GValue *result)
{
    g_value_init(result, GST_TYPE_ARRAY);

    while (type != 0)
    {
        gst_array_prepend_static_string(result, g_type_name(type));

        type = g_type_parent(type);
    }
}

void parse_type_interfaces(GType type, GValue *result)
{
    guint n_ifaces;
    GType *ifaces = g_type_interfaces(type, &n_ifaces);

    if (ifaces)
    {
        if (n_ifaces)
        {
            g_value_init(result, GST_TYPE_ARRAY);

            GType *iface = ifaces;
            while (*iface)
            {
                gst_array_append_static_string(result, g_type_name(*iface));
                iface++;
            }
        }
        g_free(ifaces);
    }
}

gboolean gtype_needs_ptr_marker(GType type)
{
    if (type == G_TYPE_POINTER)
    {
        return FALSE;
    }
    if (G_TYPE_FUNDAMENTAL(type) == G_TYPE_POINTER ||
        G_TYPE_IS_BOXED(type) ||
        G_TYPE_IS_OBJECT(type))
    {
        return TRUE;
    }
    return FALSE;
}

GstStructure *parse_signal(GSignalQuery *query)
{
    GstStructure *signal_dict = gst_structure_new_empty("signal");
    GValue params_array = G_VALUE_INIT;
    g_value_init(&params_array, GST_TYPE_ARRAY);

    gst_dictionary_set_string(signal_dict, "Signal name", g_strdup(query->signal_name));

    gst_dictionary_set_string(signal_dict, "Return type",
                              g_strdup_printf("%s%s", g_type_name(query->return_type),
                                              gtype_needs_ptr_marker(query->return_type) ? " *" : ""));

    gst_array_append_string(&params_array, g_strdup_printf("%s* object", g_type_name(query->itype)));

    for (guint i = 0; i < query->n_params; i++)
    {
        gst_array_append_string(&params_array,
                                g_strdup_printf("%s%s arg%d", g_type_name(query->param_types[i]),
                                                gtype_needs_ptr_marker(query->param_types[i]) ? "*" : "",
                                                i));
    }

    gst_array_append_static_string(&params_array, "gpointer user_data");

    gst_dictionary_set_array(signal_dict, "Signal parameters", &params_array);

    return signal_dict;
}
