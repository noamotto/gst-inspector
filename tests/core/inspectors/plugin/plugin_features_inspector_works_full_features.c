#include "gstinspectors.h"
#include "testutils.h"

// Element

#define ELEMENT_NAME ("testelement")
#define TYPEFIND_NAME ("testtypefind")

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

// Typefind
static void fake_typefind(GstTypeFind *find)
{
    (void)find;
}

GST_PLUGIN_STATIC_DECLARE(testplugin);

static gboolean plugin_init(GstPlugin *plugin)
{
    return gst_element_register(plugin, ELEMENT_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT) &&
           gst_type_find_register(plugin, TYPEFIND_NAME, GST_RANK_NONE,
                                  (GstTypeFindFunction)&fake_typefind, NULL, NULL, NULL, NULL);
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

static gchar *element_list[] = {"testelement: testelement"};
static gchar *typefinds_list[] = {"testplugin: testtypefind: no extensions"};

int main(int argc, char *argv[])
{
    GstPlugin *plugin;
    GValue result = G_VALUE_INIT;
    GstStructure *data;
    const GValue *elements, *typefinds;
    GValue *expected_elements, *expected_typefinds;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    plugin = gst_registry_find_plugin(gst_registry_get(), PLUGIN_NAME);
    gst_inspector_inspect_plugin_features(plugin, &result);

    g_assert_true(GST_VALUE_HOLDS_STRUCTURE(&result));
    data = g_value_get_boxed(&result);

    g_assert_cmpint(gst_structure_n_fields(data), ==, 3);
    g_assert_true(gst_structure_has_field_typed(data, "Total features", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Total features"), ==, "2");

    g_assert_true(gst_structure_has_field_typed(data, "Elements", GST_TYPE_ARRAY));
    elements = gst_dictionary_get_array(data, "Elements");
    expected_elements = string_to_array(1, element_list);
    g_assert_true(gst_value_compare(expected_elements, elements) == GST_VALUE_EQUAL);
    
    g_assert_true(gst_structure_has_field_typed(data, "Typefinders", GST_TYPE_ARRAY));
    typefinds = gst_dictionary_get_array(data, "Typefinders");
    expected_typefinds = string_to_array(1, typefinds_list);
    g_assert_true(gst_value_compare(expected_typefinds, typefinds) == GST_VALUE_EQUAL);

    g_value_unset(expected_typefinds);
    g_free(expected_typefinds);
    g_value_unset(expected_elements);
    g_free(expected_elements);
    g_value_unset(&result);
    gst_object_unref(plugin);

    return 0;
}
