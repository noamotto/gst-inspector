#include "gstinspector.h"
#include "gsttestplugin.h"

#define TYPEFIND_NAME   ("testtypefind")
#define TYPE_KEY        ("Type")
#define TYPE_VALUE      ("A typefind function")

int main(int argc, char *argv[])
{
    GstPluginFeature *factory;
    GstStructure *data;
    const gchar *test_field_value;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    factory = gst_registry_find_feature(gst_registry_get(), TYPEFIND_NAME, GST_TYPE_TYPE_FIND_FACTORY);
    data = gst_inspector_inspect_plugin_feature(factory);

    g_assert_cmpstr(TYPEFIND_NAME, ==, gst_structure_get_name(data));

    test_field_value = gst_dictionary_get_string(data, TYPE_KEY);
    g_assert_nonnull(test_field_value);
    g_assert_cmpstr(test_field_value, ==, TYPE_VALUE);

    return 0;
}
