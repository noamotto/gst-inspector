#include <math.h>
#include "gstinspectors.h"

static gboolean parse_field(GQuark field_id, const GValue *value, GValue *array)
{
    gchar *str = gst_value_serialize(value);
    gchar *field = g_strdup_printf("%s: %s",
                                   g_quark_to_string(field_id),
                                   str);

    gst_array_append_string(array, field);

    g_free(str);
    return TRUE;
}

static void parse_caps(const GstCaps *caps, GValue *result)
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
            gst_dictionary_set_string(caps_dict, "Features",
                                      gst_caps_features_to_string(features));
        }
        gst_structure_foreach(structure,
                              (GstStructureForeachFunc)parse_field,
                              &fields_array);

        gst_dictionary_set_array(caps_dict, "Caps", &fields_array);

        gst_array_append_subdictionary(result, caps_dict);
    }
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

static void check_plugin_details(GstPlugin *plugin, GstStructure *dict)
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

static void parse_type_hierarchy(GType type, GValue *result)
{
    g_value_init(result, GST_TYPE_ARRAY);

    while (type != 0)
    {
        gst_array_prepend_static_string(result, g_type_name(type));

        type = g_type_parent(type);
    }
}

static void parse_type_interfaces(GType type, GValue *result)
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

static void check_pad_template(GstStaticPadTemplate *template,
                                  const GstStructure *data)
{
    GValue expected_caps = G_VALUE_INIT;

    g_assert_true(gst_structure_has_field_typed(data, "Name", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Name"), ==, template->name_template);

    g_assert_true(gst_structure_has_field_typed(data, "Direction", G_TYPE_STRING));
    switch (template->direction)
    {
    case GST_PAD_SRC:
    {
        g_assert_cmpstr(gst_dictionary_get_string(data, "Direction"), ==, "SRC");
        break;
    }
    case GST_PAD_SINK:
    {
        g_assert_cmpstr(gst_dictionary_get_string(data, "Direction"), ==, "SINK");
        break;
    }
    case GST_PAD_UNKNOWN:
    default:
    {
        g_assert_cmpstr(gst_dictionary_get_string(data, "Direction"), ==, "UNKNOWN");
        break;
    }
    }

    g_assert_true(gst_structure_has_field_typed(data, "Availability", G_TYPE_STRING));
    switch (template->presence)
    {
    case GST_PAD_ALWAYS:
    {
        g_assert_cmpstr(gst_dictionary_get_string(data, "Availability"), ==, "Always");
        break;
    }
    case GST_PAD_SOMETIMES:
    {
        g_assert_cmpstr(gst_dictionary_get_string(data, "Availability"), ==, "Sometimes");
        break;
    }
    case GST_PAD_REQUEST:
    {
        g_assert_cmpstr(gst_dictionary_get_string(data, "Availability"), ==, "On request");
        break;
    }
    default:
    {
        g_assert_cmpstr(gst_dictionary_get_string(data, "Availability"), ==, "UNKNOWN");
        break;
    }
    }

    g_assert_true(gst_structure_has_field_typed(data, "Capabilities", GST_TYPE_ARRAY));
    parse_caps(gst_static_pad_template_get_caps(template), &expected_caps);
    g_assert_true(gst_value_compare(gst_dictionary_get_array(data, "Capabilities"),
                                    &expected_caps) == GST_VALUE_EQUAL);
    g_value_unset(&expected_caps);
}
