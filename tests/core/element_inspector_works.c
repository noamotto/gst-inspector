#include "gstinspector.h"
#include "gsttestplugin.h"

#define FACTORY_NAME ("testelement")
#define TEST_NAME ("test")
#define TEST_FIELD_NAME ("Test field")
#define TEST_FIELD_VALUE ("hello")

static void fake_inspector(GstElement *element, GValue *result)
{
    (void)element;

    GstStructure *test = gst_structure_new_empty(TEST_NAME);
    gst_dictionary_set_string(test, TEST_FIELD_NAME, TEST_FIELD_VALUE);
    g_value_init(result, GST_TYPE_STRUCTURE);
    g_value_take_boxed(result, test);
}

int main(int argc, char *argv[])
{
    GstElementFactory *factory;
    GstStructure *data;
    GstStructure *inspector_data;
    const gchar *test_field_value;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    gst_inspector_register_element_inspector(fake_inspector, TEST_NAME, TEST_NAME, -1);

    factory = gst_element_factory_find(FACTORY_NAME);
    data = gst_inspector_inspect_element(factory);

    g_assert_cmpstr(FACTORY_NAME, ==, gst_structure_get_name(data));

    inspector_data = gst_dictionary_get_sub_dictionary(data, TEST_NAME);
    g_assert_nonnull(inspector_data);

    test_field_value = gst_dictionary_get_string(inspector_data, TEST_FIELD_NAME);
    g_assert_cmpstr(test_field_value, ==, TEST_FIELD_VALUE);

    return 0;
}
