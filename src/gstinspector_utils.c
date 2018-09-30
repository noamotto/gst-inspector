#include "gstinspector_priv.h"
#include "type_reader/type_reader.h"
#include <math.h>

#define KEY_FEATURES ("Features")

static gboolean
print_field(
    GQuark field_id,
    const GValue *value,
    GArray *array)
{
    GValue key_value = G_VALUE_INIT;
    gchar *str = gst_value_serialize(value);
    gchar *field = g_strdup_printf("%s: %s",
                                   g_quark_to_string(field_id),
                                   str);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, field);
    g_array_append_val(array, key_value);

    g_free(str);
    return TRUE;
}

GArray *parse_caps(const GstCaps *caps)
{
    GArray *caps_array = g_array_new(FALSE, FALSE, sizeof(GValue));
    GValue caps_value = G_VALUE_INIT;

    g_array_set_clear_func(caps_array, (GDestroyNotify)g_value_unset);

    for (guint i = 0; i < gst_caps_get_size(caps); i++)
    {
        GstStructure *caps_dict = gst_structure_new_empty("caps_dict");
        GArray *fields_array = g_array_new(FALSE, TRUE, sizeof(GValue));
        GstStructure *structure = gst_caps_get_structure(caps, i);
        GstCapsFeatures *features = gst_caps_get_features(caps, i);

        gst_dictionary_set_static_string(caps_dict, KEY_TYPE, gst_structure_get_name(structure));

        if (features && (gst_caps_features_is_any(features) ||
                         !gst_caps_features_is_equal(features,
                                                     GST_CAPS_FEATURES_MEMORY_SYSTEM_MEMORY)))
        {
            gst_dictionary_set_string(caps_dict, KEY_FEATURES,
                                      gst_caps_features_to_string(features));
        }
        gst_structure_foreach(structure,
                              (GstStructureForeachFunc)print_field,
                              fields_array);
        g_array_set_clear_func(fields_array, (GDestroyNotify)g_value_unset);

        gst_dictionary_set_array(caps_dict, KEY_VALUE, fields_array);

        g_value_init(&caps_value, GST_TYPE_STRUCTURE);
        g_value_take_boxed(&caps_value, caps_dict);
        g_array_append_val(caps_array, caps_value);
    }

    return caps_array;
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

    return g_strdup_printf("%s %c %d", rank_names[best_match],
                           (rank - ranks[best_match] > 0) ? '+' : '-',
                           abs(ranks[best_match] - rank));
}

GArray *parse_type_hierarchy(GType type)
{
    GArray *hierarchy = g_array_new(FALSE, FALSE, sizeof(GValue));
    g_array_set_clear_func(hierarchy, (GDestroyNotify)g_value_unset);

    while (type != 0)
    {
        GValue hierarchy_node = G_VALUE_INIT;
        g_value_init(&hierarchy_node, G_TYPE_STRING);

        g_value_set_static_string(&hierarchy_node, g_type_name(type));
        g_array_prepend_val(hierarchy, hierarchy_node);

        type = g_type_parent(type);
    }

    return hierarchy;
}

GArray *parse_type_interfaces(GType type)
{
    guint n_ifaces;
    GArray *result = NULL;
    GType *ifaces = g_type_interfaces(type, &n_ifaces);

    if (ifaces)
    {
        if (n_ifaces)
        {
            result = g_array_new(FALSE, FALSE, sizeof(GValue));
            g_array_set_clear_func(result, (GDestroyNotify)g_value_unset);

            GType *iface = ifaces;
            while (*iface)
            {
                g_array_add_static_string(result, g_type_name(*iface));
                iface++;
            }
        }
        g_free(ifaces);
    }

    return result;
}
