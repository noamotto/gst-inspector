#include <math.h>
#include "gstinspectors.h"

static void compare_arrays(GArray *arr1, GArray *arr2)
{
    g_assert_true(arr1->len == arr2->len);

    for (guint i = 0; i < arr1->len; ++i)
    {
        GValue val1 = g_array_index(arr1, GValue, i);
        GValue val2 = g_array_index(arr2, GValue, i);
        if (G_VALUE_HOLDS(&val1, G_TYPE_ARRAY))
        {
            compare_arrays(g_value_get_boxed(&val1), g_value_get_boxed(&val2));
        }
        else
        {
            g_assert_true(gst_value_compare(&val1, &val2) == GST_VALUE_EQUAL);
        }
    }
}

static gboolean print_field(GQuark field_id,
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

static GArray *parse_caps(const GstCaps *caps)
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

        gst_dictionary_set_static_string(caps_dict, "Type", gst_structure_get_name(structure));

        if (features && (gst_caps_features_is_any(features) ||
                         !gst_caps_features_is_equal(features,
                                                     GST_CAPS_FEATURES_MEMORY_SYSTEM_MEMORY)))
        {
            gst_dictionary_set_string(caps_dict, "Features",
                                      gst_caps_features_to_string(features));
        }
        gst_structure_foreach(structure,
                              (GstStructureForeachFunc)print_field,
                              fields_array);
        g_array_set_clear_func(fields_array, (GDestroyNotify)g_value_unset);

        gst_dictionary_set_array(caps_dict, "Caps", fields_array);

        g_value_init(&caps_value, GST_TYPE_STRUCTURE);
        g_value_take_boxed(&caps_value, caps_dict);
        g_array_append_val(caps_array, caps_value);
    }

    return caps_array;
}

static gchar *parse_release_date(const gchar *date)
{
    gchar *result = NULL;
    if (date != NULL)
    {
        gchar *release_date_str, *separator;

        /* may be: YYYY-MM-DD or YYYY-MM-DDTHH:MMZ */
        /* YYYY-MM-DDTHH:MMZ => YYYY-MM-DD HH:MM (UTC) */
        release_date_str = g_strdup(date);
        separator = g_strstr_len(release_date_str, -1, "T");
        if (separator != NULL)
        {
            *separator = ' ';
            separator = g_strstr_len(separator + 1, -1, "Z");
            if (separator != NULL)
            {
                *separator = ' ';
            }

            result = g_strdup_printf("%s(UTC)", release_date_str);
            g_free(release_date_str);
        }
        else
        {
            result = release_date_str;
        }
    }

    return result;
}

static void check_plugin_detais(GstPlugin *plugin, GstStructure *dict)
{
    const gchar *filename = gst_plugin_get_filename(plugin);
    gchar *release_date = parse_release_date(gst_plugin_get_release_date_string(plugin));

    g_assert_true(gst_structure_has_field_typed(dict, "Name", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dict, "Name"),
                    ==, gst_plugin_get_name(plugin));

    g_assert_true(gst_structure_has_field_typed(dict, "Description", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dict, "Description"),
                    ==, gst_plugin_get_description(plugin));

    g_assert_true(gst_structure_has_field_typed(dict, "Filename", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dict, "Filename"),
                    ==, filename ? filename : "(null)");

    g_assert_true(gst_structure_has_field_typed(dict, "Version", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dict, "Version"), ==,
                    gst_plugin_get_version(plugin));

    g_assert_true(gst_structure_has_field_typed(dict, "License", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dict, "License"),
                    ==, gst_plugin_get_license(plugin));

    g_assert_true(gst_structure_has_field_typed(dict, "Source module", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dict, "Source module"),
                    ==, gst_plugin_get_source(plugin));

    if (release_date)
    {
        g_assert_true(gst_structure_has_field_typed(dict, "Source release date",
                                                    G_TYPE_STRING));
        g_assert_cmpstr(gst_dictionary_get_string(dict, "Source release date"),
                        ==, release_date);
        g_free(release_date);
    }
    else
    {
        g_assert_false(gst_structure_has_field(dict, "Source release date"));
    }

    g_assert_true(gst_structure_has_field_typed(dict, "Binary package", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dict, "Binary package"), ==,
                    gst_plugin_get_package(plugin));

    g_assert_true(gst_structure_has_field_typed(dict, "Origin URL", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dict, "Origin URL"),
                    ==, gst_plugin_get_origin(plugin));
}

static gchar *get_rank_name(gint rank)
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

static GArray *parse_type_hierarchy(GType type)
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

static GArray *parse_type_interfaces(GType type)
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
