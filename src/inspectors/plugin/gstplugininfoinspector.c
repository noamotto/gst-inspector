/**
 *  @file gstplugininfoinspector.c
 *  @brief Plugin information inspector implementation
 */
#include "gstinspectors.h"
#include "gstinspector_priv.h"

/**
 *  @brief Inspects plugin information
 * 
 *  Inspects the information about a single plugin
 * 
 *  @param plugin Plugin to inspect
 *  @param result 
 *  @parblock
 *  The inspected data
 * 
 *  The inspected data consists of these fields:
 *  - <b>Name</b> - Plugin's name
 *  - <b>Description</b> - Plugin's description
 *  - <b>Filename</b> - Path to where the plugin's file resides
 *  - <b>Version</b> - Plugin's version
 *  - <b>License</b> - Plugin's license
 *  - <b>Source module</b> - Source module the plugin belongs to
 *  - <b>Source release date</b> - Release date of plugin's source module, if 
 *      available
 *  - <b>Binary package</b> - Shipped package the plugin belongs to
 *  - <b>Origin URL</b> - URL to the plugin's provider
 *  @endparblock
 */
void gst_inspector_inspect_plugin_details(GstPlugin *plugin, GValue *result)
{
    GstStructure *dictionary;
    const gchar *release_date = gst_plugin_get_release_date_string(plugin);
    const gchar *filename = gst_plugin_get_filename(plugin);

    g_return_if_fail(GST_IS_PLUGIN(plugin));

    g_value_init(result, GST_TYPE_STRUCTURE);
    dictionary = gst_structure_new_empty("plugindetails");

    gst_dictionary_set_static_string(dictionary, "Name", gst_plugin_get_name(plugin));
    gst_dictionary_set_static_string(dictionary, "Description", gst_plugin_get_description(plugin));
    gst_dictionary_set_static_string(dictionary, "Filename", (filename != NULL) ? filename : "(null)");
    gst_dictionary_set_static_string(dictionary, "Version", gst_plugin_get_version(plugin));
    gst_dictionary_set_static_string(dictionary, "License", gst_plugin_get_license(plugin));
    gst_dictionary_set_static_string(dictionary, "Source module", gst_plugin_get_source(plugin));

    if (release_date != NULL)
    {
        gchar *release_date_str, *separator;

        /* may be: YYYY-MM-DD or YYYY-MM-DDTHH:MMZ */
        /* YYYY-MM-DDTHH:MMZ => YYYY-MM-DD HH:MM (UTC) */
        release_date_str = g_strdup(release_date);
        separator = g_strstr_len(release_date_str, -1, "T");
        if (separator != NULL)
        {
            *separator = ' ';
            separator = g_strstr_len(separator + 1, -1, "Z");
            if (separator != NULL)
            {
                *separator = ' ';
            }
            gst_dictionary_set_string(dictionary, "Source release date",
                                      g_strdup_printf("%s(UTC)", release_date_str));
            g_free(release_date_str);
        }
        else
        {
            gst_dictionary_set_string(dictionary, "Source release date", release_date_str);
        }
    }
    gst_dictionary_set_static_string(dictionary, "Binary package", gst_plugin_get_package(plugin));
    gst_dictionary_set_static_string(dictionary, "Origin URL", gst_plugin_get_origin(plugin));

    g_value_take_boxed(result, dictionary);
}
