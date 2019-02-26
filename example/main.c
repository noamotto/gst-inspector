/**
 *  Example of gst-inspector library. Pretty much a clone of gst-inspect I hope....
 */

#include <gstinspector.h>
#include <locale.h>

int main(int argc, char *argv[])
{
    gboolean print_all = FALSE;
    gboolean do_print_blacklist = FALSE;
    gboolean plugin_name = FALSE;
    gboolean uri_handlers = FALSE;
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
        int exit_code;

        if (argc == 1)
        {
            g_printerr("--exists requires an extra command line argument\n");
            exit_code = -1;
        }
        else
        {
            if (!plugin_name)
            {
                GstPluginFeature *feature;

                feature = gst_registry_lookup_feature(gst_registry_get(), argv[1]);
                if (feature != NULL && gst_plugin_feature_check_version(feature,
                                                                        minver_maj, minver_min, minver_micro))
                {
                    exit_code = 0;
                }
                else
                {
                    exit_code = 1;
                }

                if (feature)
                    gst_object_unref(feature);
            }
            else
            {
                /* FIXME: support checking for plugins too */
                g_printerr("Checking for plugins is not supported yet\n");
                exit_code = -1;
            }
        }
        return exit_code;
    }

    /* if no arguments, print out list of elements */
    if (argc == 1 || print_all)
    {
        if (do_print_blacklist)
        {
            print_blacklist();
        }
        else
        {
            if (print_aii)
                print_all_plugin_automatic_install_info();
            else
                print_element_list(print_all);
        }
    }
    else
    {
        /* else we try to get a factory */
        GstElementFactory *factory;
        GstPlugin *plugin;
        const char *arg = argv[argc - 1];
        int retval;

        if (!plugin_name)
        {
            factory = gst_element_factory_find(arg);

            /* if there's a factory, print out the info */
            if (factory)
            {
                retval = print_element_info(factory, print_all);
                gst_object_unref(factory);
            }
            else
            {
                retval = print_element_features(arg);
            }
        }
        else
        {
            retval = -1;
        }

        /* otherwise check if it's a plugin */
        if (retval)
        {
            plugin = gst_registry_find_plugin(gst_registry_get(), arg);

            /* if there is such a plugin, print out info */
            if (plugin)
            {
                if (print_aii)
                {
                    print_plugin_automatic_install_info(plugin);
                }
                else
                {
                    print_plugin_info(plugin);
                    print_plugin_features(plugin);
                }
            }
            else
            {
                GError *error = NULL;

                if (g_file_test(arg, G_FILE_TEST_EXISTS))
                {
                    plugin = gst_plugin_load_file(arg, &error);

                    if (plugin)
                    {
                        if (print_aii)
                        {
                            print_plugin_automatic_install_info(plugin);
                        }
                        else
                        {
                            print_plugin_info(plugin);
                            print_plugin_features(plugin);
                        }
                    }
                    else
                    {
                        g_printerr(_("Could not load plugin file: %s\n"), error->message);
                        g_clear_error(&error);
                        return -1;
                    }
                }
                else
                {
                    g_printerr(_("No such element or plugin '%s'\n"), arg);
                    return -1;
                }
            }
        }
    }

    return 0;
}
