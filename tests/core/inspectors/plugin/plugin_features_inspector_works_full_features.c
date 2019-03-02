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

// Test tracer
typedef struct _GstTestTracer
{
    GstTracer tracer;
} GstTestTracer;

typedef struct _GstTestTracerClass
{
    GstTracerClass klass;
} GstTestTracerClass;

#define GST_TYPE_TEST_TRACER gst_test_tracer_get_type()
GType gst_test_tracer_get_type(void);

G_DEFINE_TYPE(GstTestTracer, gst_test_tracer, GST_TYPE_TRACER)

void gst_test_tracer_class_init(GstTestTracerClass *klass)
{
    (void)klass;
}

void gst_test_tracer_init(GstTestTracer *self)
{
    (void)self;
}

// Test device provider
typedef struct _GstTestDeviceProvider
{
    GstDeviceProvider provider;
} GstTestDeviceProvider;

typedef struct _GstTestDeviceProviderClass
{
    GstDeviceProviderClass klass;
} GstTestDeviceProviderClass;

#define GST_TYPE_TEST_DEVICE_PROVIDER gst_test_device_provider_get_type()
GType gst_test_device_provider_get_type(void);

G_DEFINE_TYPE(GstTestDeviceProvider, gst_test_device_provider, GST_TYPE_DEVICE_PROVIDER)

void gst_test_device_provider_class_init(GstTestDeviceProviderClass *klass)
{
    gst_device_provider_class_set_static_metadata(GST_DEVICE_PROVIDER_CLASS(klass),
                                                  "testprovider",
                                                  "TEST",
                                                  "Test Device Provider",
                                                  "Noam Ottolenghi");
}

void gst_test_device_provider_init(GstTestDeviceProvider *self)
{
    (void)self;
}

// Typefind
static void fake_typefind(GstTypeFind *find)
{
    (void)find;
}

static void fake_typefind2(GstTypeFind *find)
{
    (void)find;
}

GST_PLUGIN_STATIC_DECLARE(testplugin);

static gboolean plugin_init(GstPlugin *plugin)
{
    return gst_element_register(plugin, ELEMENT_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT) &&
           gst_type_find_register(plugin, TYPEFIND_NAME, GST_RANK_NONE,
                                  (GstTypeFindFunction)&fake_typefind, NULL, NULL, NULL, NULL) &&
           gst_type_find_register(plugin, "testtypefind2", GST_RANK_NONE,
                                  (GstTypeFindFunction)&fake_typefind2, "a,b,c,d", NULL, NULL, NULL) &&
           gst_tracer_register(plugin, "testtracer", GST_TYPE_TEST_TRACER) &&
           gst_device_provider_register(plugin, "testprovider", GST_RANK_NONE,
                                        GST_TYPE_TEST_DEVICE_PROVIDER);
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
static gchar *typefinds_list[] = {"testplugin: testtypefind: no extensions",
                                  "testplugin: testtypefind2: a, b, c, d"};
static gchar *tracers_list[] = {"testtracer (GstTestTracer)"};
static gchar *providers_list[] = {"testprovider: testprovider"};

int main(int argc, char *argv[])
{
    GstPlugin *plugin;
    GValue result = G_VALUE_INIT;
    GstStructure *data;
    const GValue *elements, *typefinds, *tracers, *providers;
    GValue *expected_elements, *expected_typefinds, *expected_tracers, *expected_providers;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    plugin = gst_registry_find_plugin(gst_registry_get(), PLUGIN_NAME);
    gst_inspector_inspect_plugin_features(plugin, &result);

    g_assert_true(GST_VALUE_HOLDS_STRUCTURE(&result));
    data = g_value_get_boxed(&result);

    g_assert_cmpint(gst_structure_n_fields(data), ==, 5);
    g_assert_true(gst_structure_has_field_typed(data, "Total features", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Total features"), ==, "5");

    g_assert_true(gst_structure_has_field_typed(data, "Elements", GST_TYPE_LIST));
    elements = gst_dictionary_get_array(data, "Elements");
    expected_elements = string_to_array(1, element_list);
    g_assert_true(gst_value_compare(expected_elements, elements) == GST_VALUE_EQUAL);

    g_assert_true(gst_structure_has_field_typed(data, "Typefinders", GST_TYPE_LIST));
    typefinds = gst_dictionary_get_array(data, "Typefinders");
    expected_typefinds = string_to_array(2, typefinds_list);
    g_assert_true(gst_value_compare(expected_typefinds, typefinds) == GST_VALUE_EQUAL);

    g_assert_true(gst_structure_has_field_typed(data, "Tracers", GST_TYPE_LIST));
    tracers = gst_dictionary_get_array(data, "Tracers");
    expected_tracers = string_to_array(1, tracers_list);
    g_assert_true(gst_value_compare(expected_tracers, tracers) == GST_VALUE_EQUAL);

    g_assert_true(gst_structure_has_field_typed(data, "Device providers", GST_TYPE_LIST));
    providers = gst_dictionary_get_array(data, "Device providers");
    expected_providers = string_to_array(1, providers_list);
    g_assert_true(gst_value_compare(expected_providers, providers) == GST_VALUE_EQUAL);

    g_value_unset(expected_providers);
    g_free(expected_providers);
    g_value_unset(expected_tracers);
    g_free(expected_tracers);
    g_value_unset(expected_typefinds);
    g_free(expected_typefinds);
    g_value_unset(expected_elements);
    g_free(expected_elements);
    g_value_unset(&result);
    gst_object_unref(plugin);

    return 0;
}
