#include "gstinspectors.h"
#include "testutils.h"

#define FACTORY_NAME ("testelement")

typedef struct _GstTestElement
{
    GstElement element;
} GstTestElement;

typedef struct _GstTestElementClass
{
    GstElementClass klass;
} GstTestElementClass;

#define GST_TYPE_TEST_ELEMENT gst_test_element_get_type()
GType gst_test_element_get_type(void);

G_DEFINE_TYPE(GstTestElement, gst_test_element, GST_TYPE_ELEMENT)

void gst_test_element_class_init(GstTestElementClass *klass)
{
    gst_element_class_set_static_metadata(GST_ELEMENT_CLASS(klass),
                                          "testelement",
                                          "TEST",
                                          "Test Element",
                                          "Noam Ottolenghi");
}

void gst_test_element_init(GstTestElement *self)
{
    (void)self;
}

static gboolean plugin_init(GstPlugin *plugin)
{
    return gst_element_register(plugin, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);
}

GST_PLUGIN_STATIC_DECLARE(testplugin);

void gst_plugin_testplugin_register()
{
    gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR, "testplugin",
                               "Test Plugin", plugin_init, "0.0.1", GST_LICENSE_UNKNOWN,
                               "none", "test", "here");
}

int main(int argc, char *argv[])
{
    GstElement *element;
    GstElementFactory *factory;
    GstPlugin *plugin;
    GValue result = G_VALUE_INIT;
    GstStructure *data;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    factory = gst_element_get_factory(element);
    plugin = gst_plugin_feature_get_plugin(GST_PLUGIN_FEATURE(factory));
    gst_inspector_inspect_element_plugin(element, &result);

    g_assert_true(GST_VALUE_HOLDS_STRUCTURE(&result));
    data = (GstStructure *)g_value_get_boxed(&result);

    check_plugin_details(plugin, data);

    g_value_unset(&result);
    gst_object_unref(element);
    gst_deinit();

    return 0;
}
