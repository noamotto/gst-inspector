#include "gstinspectors.h"

#define PLUGIN_NAME ("testplugindatetime")
#define PLUGIN_DESC ("Test Plugin")

int main(int argc, char *argv[])
{
    GstPlugin *plugin;
    GValue result = G_VALUE_INIT;
    GstStructure *data;

    gst_init(&argc, &argv);

    plugin = gst_registry_find_plugin(gst_registry_get(), PLUGIN_NAME);
    gst_inspector_inspect_plugin_details(plugin, &result);

    g_assert_true(GST_VALUE_HOLDS_STRUCTURE(&result));
    data = g_value_get_boxed(&result);

    g_assert_true(gst_structure_has_field_typed(data, "Name", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Name"), ==, PLUGIN_NAME);
    
    g_assert_true(gst_structure_has_field_typed(data, "Description", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Description"), ==, PLUGIN_DESC);
    
    g_assert_true(gst_structure_has_field_typed(data, "Filename", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Filename"), ==, gst_plugin_get_filename(plugin));
    
    g_assert_true(gst_structure_has_field_typed(data, "Version", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Version"), ==, "0.0.1");
    
    g_assert_true(gst_structure_has_field_typed(data, "License", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "License"), ==, GST_LICENSE_UNKNOWN);

    g_assert_true(gst_structure_has_field_typed(data, "Source module", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Source module"), ==, "none");

    g_assert_true(gst_structure_has_field_typed(data, "Source release date", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Source release date"), ==, "2018-10-05 12:32 (UTC)");
    
    g_assert_true(gst_structure_has_field_typed(data, "Binary package", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Binary package"), ==, "test");

    g_assert_true(gst_structure_has_field_typed(data, "Origin URL", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Origin URL"), ==, "here");

    g_value_unset(&result);
    gst_object_unref(plugin);

    return 0;
}
