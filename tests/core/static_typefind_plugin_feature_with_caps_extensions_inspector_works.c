#include "gstinspector.h"
#include "testutils.h"

#define TYPEFIND_NAME ("testtypefind")

static GstStaticCaps caps_templ = GST_STATIC_CAPS("video/x-raw, width=320, height=240");

static void fake_typefind(GstTypeFind *find)
{
    (void)find;
}

int main(int argc, char *argv[])
{
    GstPluginFeature *feature;
    GstStructure *data, *factory_data;
    guint rank;
    gchar *rank_string;
    GstCaps *caps;
    GValue caps_arr = G_VALUE_INIT;
    GValue extensions_arr = G_VALUE_INIT;

    gst_init(&argc, &argv);

    caps = gst_static_caps_get(&caps_templ);
    gst_type_find_register(NULL, "testtypefind", GST_RANK_NONE,
                           (GstTypeFindFunction)fake_typefind, "a,b,c", caps, NULL, NULL);

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

    parse_caps(caps, &caps_arr);
    g_assert_true(gst_structure_has_field_typed(data, "Caps", GST_TYPE_LIST));
    g_assert_true(gst_value_compare(&caps_arr, gst_dictionary_get_array(data, "Caps")) == GST_VALUE_EQUAL);
    g_value_unset(&caps_arr);

    g_value_init(&extensions_arr, GST_TYPE_LIST);
    gst_array_append_static_string(&extensions_arr, "a");
    gst_array_append_static_string(&extensions_arr, "b");
    gst_array_append_static_string(&extensions_arr, "c");
    g_assert_true(gst_structure_has_field_typed(data, "Extensions", GST_TYPE_LIST));
    g_assert_true(gst_value_compare(&extensions_arr, gst_dictionary_get_array(data, "Extensions")) == GST_VALUE_EQUAL);
    g_value_unset(&extensions_arr);

    g_assert_false(gst_structure_has_field(data, "Plugin Details"));

    gst_object_unref(feature);
    gst_caps_unref(caps);
    gst_structure_free(data);

    return 0;
}
