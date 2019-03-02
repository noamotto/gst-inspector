#include "gstinspector.h"
#include "gsttestplugin.h"
#include "testutils.h"

#define TYPEFIND_NAME ("testtypefind")

int main(int argc, char *argv[])
{
    GstPluginFeature *feature;
    GstStructure *data, *factory_data;
    guint rank;
    gchar *rank_string;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    feature = gst_registry_find_feature(gst_registry_get(), TYPEFIND_NAME,
                                        GST_TYPE_TYPE_FIND_FACTORY);
    data = gst_inspector_inspect_plugin_feature(feature);

    g_assert_cmpstr(TYPEFIND_NAME, ==, gst_structure_get_name(data));

    g_assert_true(gst_structure_has_field_typed(data, "Factory Details", GST_TYPE_STRUCTURE));
    factory_data = gst_dictionary_get_sub_dictionary(data, "Factory Details");

    g_assert_true(gst_structure_has_field_typed(factory_data, "Name", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(factory_data, "Name"), ==, TYPEFIND_NAME);

    g_assert_true(gst_structure_has_field_typed(factory_data, "Rank", G_TYPE_STRING));
    rank = gst_plugin_feature_get_rank(feature);
    rank_string = get_rank_name((gint)rank);
    g_assert_cmpstr(gst_dictionary_get_string(factory_data, "Rank"), ==, rank_string);
    g_free(rank_string);

    g_assert_false(gst_structure_has_field(factory_data, "Caps"));

    g_assert_false(gst_structure_has_field(factory_data, "Extensions"));

    g_assert_true(gst_structure_has_field_typed(data, "Plugin Details", GST_TYPE_STRUCTURE));
    check_plugin_details(gst_plugin_feature_get_plugin(feature),
                         gst_dictionary_get_sub_dictionary(data, "Plugin Details"));

    gst_object_unref(feature);
    gst_structure_free(data);
    gst_deinit();

    return 0;
}
