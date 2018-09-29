#include "gstinspectors.h"

#define FACTORY_NAME ("testelement")
#define TEST_PROP_ID (1)

void test_get_property(GObject *object,
                       guint property_id,
                       GValue *value,
                       GParamSpec *pspec);

void test_set_property(GObject *object,
                       guint property_id,
                       const GValue *value,
                       GParamSpec *pspec);

#define TEST_ELEMENT_CLASS_INIT_CODE                                       \
    G_OBJECT_CLASS(klass)->get_property = test_get_property;               \
    G_OBJECT_CLASS(klass)->set_property = test_set_property;               \
    g_object_class_install_property(G_OBJECT_CLASS(klass), TEST_PROP_ID,   \
                                    g_param_spec_boolean("param", "param", \
                                                         "param", TRUE, G_PARAM_WRITABLE));

#include "../gsttestplugin.h"

void test_get_property(GObject *object,
                       guint property_id,
                       GValue *value,
                       GParamSpec *pspec)
{
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
}

void test_set_property(GObject *object,
                       guint property_id,
                       const GValue *value,
                       GParamSpec *pspec)
{
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
}

int main(int argc, char *argv[])
{
    GstElementFactory *factory;
    GstStructure *data;
    GstStructure *inspector_data;
    GstStructure *param_data;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    gst_inspector_register_element_inspector(param_inspector, "params", "Element Properties", -1);

    factory = gst_element_factory_find(FACTORY_NAME);
    data = gst_inspector_inspect_element(factory);

    g_assert_cmpstr(FACTORY_NAME, ==, gst_structure_get_name(data));

    inspector_data = gst_dictionary_get_sub_dictionary(data, "Element Properties");
    g_assert_nonnull(inspector_data);

    param_data = gst_dictionary_get_sub_dictionary(inspector_data, "param");
    g_assert_nonnull(param_data);

    g_assert_true(gst_structure_has_field_typed(param_data, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(param_data, KEY_NAME), ==, "param: param");

    g_assert_true(gst_structure_has_field_typed(param_data, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(param_data, KEY_TYPE), ==, "Boolean");

    g_assert_true(gst_structure_has_field_typed(param_data, KEY_VALUE, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(param_data, KEY_VALUE), ==, "true");

    return 0;
}
