#include "gstinspector.h"
#include "gsttestplugin.h"
#include "testutils.h"

#define TRACER_NAME ("testtracer")

int main(int argc, char *argv[])
{
    GValue hierarchy_arr = G_VALUE_INIT;
    GstPluginFeature *factory;
    GstStructure *data;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    factory = gst_registry_find_feature(gst_registry_get(), TRACER_NAME, GST_TYPE_TRACER_FACTORY);
    data = gst_inspector_inspect_plugin_feature(factory);

    g_assert_cmpstr(TRACER_NAME, ==, gst_structure_get_name(data));

    g_assert_true(gst_structure_has_field_typed(data, "Factory Name", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Factory Name"), ==, TRACER_NAME);

    g_assert_true(gst_structure_has_field_typed(data, "Plugin Details", GST_TYPE_STRUCTURE));
    check_plugin_details(gst_plugin_feature_get_plugin(factory),
                         gst_dictionary_get_sub_dictionary(data, "Plugin Details"));

    parse_type_hierarchy(gst_tracer_factory_get_tracer_type(GST_TRACER_FACTORY(factory)),
                         &hierarchy_arr);
    g_assert_true(gst_structure_has_field_typed(data, "Type Hierarchy", GST_TYPE_LIST));
    g_assert_true(gst_value_compare(&hierarchy_arr,
                                    gst_dictionary_get_array(data, "Type Hierarchy")) == GST_VALUE_EQUAL);
    g_value_unset(&hierarchy_arr);

    g_assert_false(gst_structure_has_field(data, "Implemented Interfaces"));

    gst_object_unref(factory);
    gst_structure_free(data);

    return 0;
}
