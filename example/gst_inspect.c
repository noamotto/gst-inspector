/**
 *  Example of gst-inspector library. Shows some of the features in gst-inspect
 */

#include <gstinspectors.h>
#include <locale.h>
#include <stdio.h>

static gint tabs = 0;
static const gchar *print_name = NULL;

void print_field(const gchar *name, const GValue *field);
void print_array(const GValue *array);
void print_dictionary(const GstStructure *dictionary);

static void pretty_print(const gchar *format, ...)
{
    va_list args;

    if (print_name)
    {
        g_print("%s", print_name);
    }

    for (gint i = 0; i < tabs; i++)
    {
        g_print("    ");
    }

    va_start(args, format);
    g_vprintf(format, args);
    va_end(args);
}

void print_field(const gchar *name, const GValue *field)
{
    if (name != NULL)
    {
        pretty_print("%s: %s\n", name, g_value_get_string(field));
    }
    else
    {
        pretty_print("%s\n", g_value_get_string(field));
    }
}

void print_array(const GValue *array)
{
    guint length = gst_array_get_size(array);

    for (guint i = 0; i < length; i++)
    {
        const GValue *field = gst_array_get_value(array, i);

        if (G_VALUE_HOLDS_STRING(field))
        {
            print_field(NULL, field);
        }
        else if (GST_VALUE_HOLDS_LIST(field))
        {
            ++tabs;
            print_array(field);
            --tabs;
        }
        else
        {
            ++tabs;
            print_dictionary(gst_value_get_structure(field));
            --tabs;
        }

        if (!G_VALUE_HOLDS_STRING(field))
        {
            pretty_print("\n");
        }
    }
}

void print_dictionary(const GstStructure *dictionary)
{
    gint length = gst_dictionary_get_size(dictionary);

    for (gint i = 0; i < length; i++)
    {
        const gchar *field_name = gst_dictionary_get_nth_name(dictionary, (guint)i);
        const GValue *field = gst_dictionary_get_value(dictionary, field_name);

        if (G_VALUE_HOLDS_STRING(field))
        {
            print_field(field_name, field);
        }
        else if (GST_VALUE_HOLDS_LIST(field))
        {
            pretty_print("%s:\n", field_name);
            ++tabs;
            print_array(field);
            --tabs;
        }
        else
        {
            pretty_print("%s:\n", field_name);
            ++tabs;
            print_dictionary(gst_value_get_structure(field));
            --tabs;
        }
        pretty_print("\n");
    }
}

static gboolean lookup_name_in_list(const gchar *name, GValue *list)
{
    guint len;

    g_return_val_if_fail(!GST_VALUE_HOLDS_LIST(list), FALSE);

    len = gst_array_get_size(list);

    for (guint i = 0; i < len; i++)
    {
        const GValue *val = gst_array_get_value(list, i);
        const gchar *str = g_value_get_string(val);
        if (0 == g_strcmp0(name, str))
        {
            return TRUE;
        }
    }
    return FALSE;
}

static int do_check_exists(const gchar *name, const gchar *min_version, gboolean plugin_name)
{
    GValue list = G_VALUE_INIT;
    gint result = 1;
    g_value_init(&list, GST_TYPE_LIST);

    if (!plugin_name)
    {
        gst_inspector_get_installed_features((GstPluginFlags)0, min_version, &list);
    }
    else
    {
        gst_inspector_get_installed_plugins((GstPluginFlags)0, min_version, &list);
    }

    if (lookup_name_in_list(name, &list))
    {
        result = 0;
    }

    g_value_unset(&list);
    return result;
}

static void print_blacklist()
{
    GValue list = G_VALUE_INIT;
    guint len;

    g_value_init(&list, GST_TYPE_LIST);

    g_print("Blacklisted files:\n");

    gst_inspector_get_installed_plugins(GST_PLUGIN_FLAG_BLACKLISTED, NULL, &list);

    len = gst_array_get_size(&list);

    ++tabs;
    for (guint i = 0; i < len; ++i)
    {
        const GValue *field = gst_array_get_value(&list, i);
        pretty_print("%s\n", g_value_get_string(field));
    }
    --tabs;

    g_print("\n");
    g_print("Total count: ");
    g_print("%d blacklisted files\n", len);
    g_value_unset(&list);
}

static guint print_plugin_features(const GstStructure *plugin, const gchar *type)
{
    const GstStructure *data = gst_dictionary_get_sub_dictionary(plugin,
                                                                 GST_INSPECTOR_PLUGIN_FEATURES_LONGNAME);
    const GValue *feature_list = gst_dictionary_get_array(data, type);

    if (feature_list)
    {
        guint features = gst_array_get_size(feature_list);

        ++tabs;
        for (guint i = 0; i < features; ++i)
        {
            const GValue *feature = gst_array_get_value(feature_list, i);
            pretty_print("%s:  %s\n", gst_structure_get_name(plugin), g_value_get_string(feature));
        }
        --tabs;

        return features;
    }

    return 0;
}

static void print_element_list()
{
    GValue plugin_list = G_VALUE_INIT;
    guint len;
    GstRegistry *reg = gst_registry_get();
    GList *feature_list = NULL;
    guint feature_counter = 0;
    guint total_counter = 0;

    g_value_init(&plugin_list, GST_TYPE_LIST);

    // Prepare gst-inspector to print only plugin features
    gst_inspector_clear_plugin_inspectors();
    gst_inspector_register_plugin_inspector(gst_inspector_inspect_plugin_features,
                                            GST_INSPECTOR_PLUGIN_FEATURES_NAME,
                                            GST_INSPECTOR_PLUGIN_FEATURES_LONGNAME, -1);

    gst_inspector_get_installed_plugins((GstPluginFlags)0, NULL, &plugin_list);

    len = gst_array_get_size(&plugin_list);

    // Gather all features from plugins
    for (guint i = 0; i < len; i++)
    {
        const GValue *field = gst_array_get_value(&plugin_list, i);
        const gchar *plugin_name = g_value_get_string(field);
        GstPlugin *plugin = gst_registry_find_plugin(reg, plugin_name);

        feature_list = g_list_append(feature_list, gst_inspector_inspect_plugin(plugin));
        gst_object_unref(plugin);
    }

    // Pretty print elements
    pretty_print("Elements:\n");
    for (guint i = 0; i < len; i++)
    {
        feature_counter += print_plugin_features(g_list_nth_data(feature_list, i),
                                                 "Elements");
    }
    pretty_print("Total elements: %u\n\n", feature_counter);
    total_counter += feature_counter;
    feature_counter = 0;

    pretty_print("Typefinders:\n");
    for (guint i = 0; i < len; i++)
    {
        feature_counter += print_plugin_features(g_list_nth_data(feature_list, i),
                                                 "Typefinders");
    }
    pretty_print("Total typefinders: %u\n\n", feature_counter);
    total_counter += feature_counter;
    feature_counter = 0;

    pretty_print("Device providers:\n");
    for (guint i = 0; i < len; i++)
    {
        feature_counter += print_plugin_features(g_list_nth_data(feature_list, i),
                                                 "Device providers");
    }
    pretty_print("Total device providers: %u\n\n", feature_counter);
    total_counter += feature_counter;
    feature_counter = 0;

    pretty_print("Tracers:\n");
    for (guint i = 0; i < len; i++)
    {
        feature_counter += print_plugin_features(g_list_nth_data(feature_list, i),
                                                 "Tracers");
    }
    pretty_print("Total tracers: %u\n\n", feature_counter);
    total_counter += feature_counter;
    feature_counter = 0;

    pretty_print("Other objects:\n");
    for (guint i = 0; i < len; i++)
    {
        feature_counter += print_plugin_features(g_list_nth_data(feature_list, i),
                                                 "Other objects");
    }
    pretty_print("Total other objects: %u\n\n", feature_counter);
    total_counter += feature_counter;

    pretty_print("Total of %u features in %u plugins", total_counter, len);

    g_list_free_full(feature_list, (GDestroyNotify)gst_structure_free);
    g_value_unset(&plugin_list);
}

static void print_all_elements()
{
}

int main(int argc, char *argv[])
{
    gboolean print_all = FALSE;
    gboolean do_print_blacklist = FALSE;
    gboolean plugin_name = FALSE;
    gboolean check_exists = FALSE;
    gchar *min_version = NULL;
    guint minver_maj = GST_VERSION_MAJOR;
    guint minver_min = GST_VERSION_MINOR;
    guint minver_micro = 0;
#ifndef GST_DISABLE_OPTION_PARSING
    GOptionEntry options[] = {
        {"print-all", 'a', 0, G_OPTION_ARG_NONE, &print_all,
         ("Print all elements"), NULL},
        {"print-blacklist", 'b', 0, G_OPTION_ARG_NONE, &do_print_blacklist,
         ("Print list of blacklisted files"), NULL},
        {"plugin", '\0', 0, G_OPTION_ARG_NONE, &plugin_name,
         ("List the plugin contents"), NULL},
        {"exists", '\0', 0, G_OPTION_ARG_NONE, &check_exists,
         ("Check if the specified element or plugin exists"), NULL},
        {"atleast-version", '\0', 0, G_OPTION_ARG_STRING, &min_version,
         ("When checking if an element or plugin exists, also check that its "
          "version is at least the version specified"),
         NULL},
        {NULL}};
    GOptionContext *ctx;
    GError *err = NULL;
#endif

    setlocale(LC_ALL, "");

    /* avoid glib warnings when inspecting deprecated properties */
    g_setenv("G_ENABLE_DIAGNOSTIC", "0", FALSE);

#ifndef GST_DISABLE_OPTION_PARSING
    ctx = g_option_context_new("[ELEMENT-NAME | PLUGIN-NAME]");
    g_option_context_add_main_entries(ctx, options, NULL);
    g_option_context_add_group(ctx, gst_init_get_option_group());
    if (!g_option_context_parse(ctx, &argc, &argv, &err))
    {
        g_printerr("Error initializing: %s\n", err->message);
        g_clear_error(&err);
        g_option_context_free(ctx);
        return -1;
    }
    g_option_context_free(ctx);
#else
    gst_init(&argc, &argv);
#endif

    if (print_all && argc > 1)
    {
        g_printerr("-a requires no extra arguments\n");
        return -1;
    }

    /* --atleast-version implies --exists */
    if (min_version != NULL)
    {
        if (sscanf(min_version, "%u.%u.%u", &minver_maj, &minver_min,
                   &minver_micro) < 2)
        {
            g_printerr("Can't parse version '%s' passed to --atleast-version\n",
                       min_version);
            return -1;
        }
        check_exists = TRUE;
    }

    if (check_exists)
    {
        if (argc == 1)
        {
            g_printerr("--exists requires an extra command line argument\n");
            return -1;
        }
        else
        {
            return do_check_exists(argv[1], min_version, plugin_name);
        }
    }

    /* if no arguments, print out list of elements */
    if (argc == 1 || print_all)
    {
        if (do_print_blacklist)
        {
            print_blacklist();
        }
        else if (print_all)
        {
            print_all_elements();
        }
        else
        {
            print_element_list();
        }
    }
    else
    {
        // /* else we try to get a factory */
        // GstElementFactory *factory;
        // GstPlugin *plugin;
        // const char *arg = argv[argc - 1];
        // int retval;

        // if (!plugin_name)
        // {
        //     factory = gst_element_factory_find(arg);

        //     /* if there's a factory, print out the info */
        //     if (factory)
        //     {
        //         retval = print_element_info(factory, print_all);
        //         gst_object_unref(factory);
        //     }
        //     else
        //     {
        //         retval = print_element_features(arg);
        //     }
        // }
        // else
        // {
        //     retval = -1;
        // }

        // /* otherwise check if it's a plugin */
        // if (retval)
        // {
        //     plugin = gst_registry_find_plugin(gst_registry_get(), arg);

        //     /* if there is such a plugin, print out info */
        //     if (plugin)
        //     {
        //         if (print_aii)
        //         {
        //             print_plugin_automatic_install_info(plugin);
        //         }
        //         else
        //         {
        //             print_plugin_info(plugin);
        //             print_plugin_features(plugin);
        //         }
        //     }
        //     else
        //     {
        //         GError *error = NULL;

        //         if (g_file_test(arg, G_FILE_TEST_EXISTS))
        //         {
        //             plugin = gst_plugin_load_file(arg, &error);

        //             if (plugin)
        //             {
        //                 if (print_aii)
        //                 {
        //                     print_plugin_automatic_install_info(plugin);
        //                 }
        //                 else
        //                 {
        //                     print_plugin_info(plugin);
        //                     print_plugin_features(plugin);
        //                 }
        //             }
        //             else
        //             {
        //                 g_printerr(_("Could not load plugin file: %s\n"), error->message);
        //                 g_clear_error(&error);
        //                 return -1;
        //             }
        //         }
        //         else
        //         {
        //             g_printerr(_("No such element or plugin '%s'\n"), arg);
        //             return -1;
        //         }
        //     }
        // }
    }

    return 0;
}
