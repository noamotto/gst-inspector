#include "gstinspectors.h"

GST_PLUGIN_STATIC_DECLARE(testplugin);

static gboolean plugin_init(GstPlugin *plugin)
{
    (void)plugin;
    return TRUE;
}

void gst_plugin_testplugin_register(void)
{
    gst_plugin_register_static(GST_VERSION_MAJOR,
                               GST_VERSION_MINOR,
                               "testplugin",
                               "Test Plugin",
                               plugin_init,
                               "0.0.1",
                               GST_LICENSE_UNKNOWN,
                               "none",
                               "test",
                               "here");
}

#define PLUGIN_NAME ("testplugin")
#define PLUGIN_DESC ("Test Plugin")

int main(int argc, char *argv[])
{
    GstPlugin *plugin;
    GValue result = G_VALUE_INIT;
    GstStructure *data;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    plugin = gst_registry_find_plugin(gst_registry_get(), PLUGIN_NAME);
    gst_inspector_inspect_plugin_features(plugin, &result);

    g_assert_true(GST_VALUE_HOLDS_STRUCTURE(&result));
    data = g_value_get_boxed(&result);

    g_assert_cmpint(gst_structure_n_fields(data), ==, 1);
    g_assert_true(gst_structure_has_field_typed(data, "Total features", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Total features"), ==, "0");

    g_value_unset(&result);
    gst_object_unref(plugin);
    gst_deinit();

    return 0;
}
