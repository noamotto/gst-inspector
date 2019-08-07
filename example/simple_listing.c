#include <gstinspector.h>
#include <locale.h>

static gint tabs = 0;

void print_usage(void);
void print_field(const gchar *field);
void print_array(const GValue *array);
void print_dictionary(const GstStructure *dictionary);

void print_usage()
{
    g_print("Usage: simple_listing <plugin|feature>");
}

void print_field(const gchar *name, const GValue *field)
{
    for (gint i = 0; i < tabs; i++)
    {
        g_print("    ");
    }

    if (name != NULL)
    {
        g_print("%s: %s\n", name, g_value_get_string(field));
    }
    else
    {
        g_print("%s\n", g_value_get_string(field));
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
            g_print("%s:\n", field_name);
            ++tabs;
            print_array(field);
            --tabs;
        }
        else
        {
            g_print("%s:\n", field_name);
            ++tabs;
            print_dictionary(gst_value_get_structure(field));
            --tabs;
        }
        g_print("\n");
    }
}

int main(int argc, char *argv[])
{
    gboolean do_print_blacklist = FALSE;
    gchar *min_version = NULL;

    GValue plugin_list = G_VALUE_INIT;
    GValue feature_list = G_VALUE_INIT;
    GstPluginFlags flags = 0;

#ifndef GST_DISABLE_OPTION_PARSING
    GOptionEntry options[] = {
        {"print-blacklist", 'b', 0, G_OPTION_ARG_NONE, &do_print_blacklist,
         ("Print list of blacklisted files"), NULL},
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
    ctx = g_option_context_new(NULL);
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

    g_value_init(&plugin_list, GST_TYPE_LIST);
    g_value_init(&feature_list, GST_TYPE_LIST);

    if (do_print_blacklist)
    {
        flags |= GST_PLUGIN_FLAG_BLACKLISTED;
    }

    gst_inspector_get_installed_plugins(flags, min_version, &plugin_list);
    g_print("Plugins:\n");
    g_print("============\n\n");
    print_array(&plugin_list);
    g_print("\n\n");

    gst_inspector_get_installed_features(flags, min_version, &feature_list);
    g_print("Features:\n");
    g_print("============\n\n");
    print_array(&feature_list);
    
    g_value_unset(&plugin_list);
    g_value_unset(&feature_list);

    return 0;
}
