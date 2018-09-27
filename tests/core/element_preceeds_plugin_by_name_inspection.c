#include "gstinspector.h"
#include "gsttestplugin.h"

#define FACTORY_NAME ("testplugin")
#define ELEMENT_TEST_NAME ("ElementTest")
#define PLUGIN_TEST_NAME ("PluginTest")
#define TEST_FIELD_NAME ("Test field")
#define TEST_FIELD_VALUE ("hello")

// Another element implementation
typedef struct _GstDuplicateElement
{
    GstElement element;
} GstDuplicateElement;

typedef struct _GstDuplicateElementClass
{
    GstElementClass klass;
} GstDuplicateElementClass;

#define GST_TYPE_DUPLICATE_ELEMENT (gst_duplicate_element_get_type())
GType gst_duplicate_element_get_type(void);
G_DEFINE_TYPE(GstDuplicateElement, gst_duplicate_element, GST_TYPE_ELEMENT)

void gst_duplicate_element_class_init(GstDuplicateElementClass *klass)
{
    gst_element_class_set_static_metadata(GST_ELEMENT_CLASS(klass),
                                          "testplugin",
                                          "TEST",
                                          "Test Duplicate Element",
                                          "Noam Ottolenghi");

    TEST_ELEMENT_CLASS_INIT_CODE
}

void gst_duplicate_element_init(GstDuplicateElement *self)
{
    TEST_ELEMENT_INIT_CODE
}

static GstStructure *fake_element_inspector(GstElement *element)
{
    (void)element;
    GstStructure *test = gst_structure_new_empty(ELEMENT_TEST_NAME);
    gst_dictionary_set_string(test, TEST_FIELD_NAME, TEST_FIELD_VALUE);
    return test;
}

static GstStructure *fake_plugin_inspector(GstPlugin *plugin)
{
    (void)plugin;
    GstStructure *test = gst_structure_new_empty(PLUGIN_TEST_NAME);
    gst_dictionary_set_string(test, TEST_FIELD_NAME, TEST_FIELD_VALUE);
    return test;
}

int main(int argc, char *argv[])
{
    GstStructure *data;
    GstStructure *inspector_data;
    gchar *expected_message = g_strdup_printf("Could not find object named %s", FACTORY_NAME);

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    gst_element_register(NULL, "testplugin", GST_RANK_NONE, GST_TYPE_DUPLICATE_ELEMENT);

    gst_inspector_register_element_inspector(fake_element_inspector, ELEMENT_TEST_NAME, ELEMENT_TEST_NAME, -1);
    gst_inspector_register_plugin_inspector(fake_plugin_inspector, PLUGIN_TEST_NAME, PLUGIN_TEST_NAME, -1);

    data = gst_inspector_inspect_by_name(FACTORY_NAME);

    g_assert_cmpstr(FACTORY_NAME, ==, gst_structure_get_name(data));

    inspector_data = gst_dictionary_get_sub_dictionary(data, ELEMENT_TEST_NAME);
    g_assert_nonnull(inspector_data);

    return 0;
}
