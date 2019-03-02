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

void gst_test_element_preset_init(GstPresetInterface *iface);
gchar ** gst_test_element_get_preset_names(GstPreset *preset);

G_DEFINE_TYPE_WITH_CODE(GstTestElement, gst_test_element, GST_TYPE_ELEMENT,
                        G_IMPLEMENT_INTERFACE(GST_TYPE_PRESET, gst_test_element_preset_init))

void gst_test_element_class_init(GstTestElementClass *klass)
{
    gst_element_class_set_static_metadata(GST_ELEMENT_CLASS(klass),
                                          "testelement",
                                          "TEST",
                                          "Test Element",
                                          "Noam Ottolenghi");
}

void gst_test_element_preset_init(GstPresetInterface *iface)
{
    iface->get_preset_names = gst_test_element_get_preset_names;
}

#define PRESET_1 "preset1"
#define PRESET_2 "preset2"
gchar ** gst_test_element_get_preset_names(GstPreset *preset)
{
    gchar **presets = g_new0(gchar *, 3);
    
    (void)preset;
    presets[0] = g_strdup(PRESET_1);
    presets[1] = g_strdup(PRESET_2);
    return presets;
}

void gst_test_element_init(GstTestElement *self)
{
    (void)self;
}

int main(int argc, char *argv[])
{
    GstElement *element;
    GValue result = G_VALUE_INIT;
    GValue expected = G_VALUE_INIT;

    gst_init(&argc, &argv);
    gst_element_register(NULL, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    gst_inspector_inspect_element_presets(element, &result);

    g_assert_true(GST_VALUE_HOLDS_LIST(&result));
    g_value_init(&expected, GST_TYPE_LIST);
    gst_array_append_static_string(&expected, PRESET_1);
    gst_array_append_static_string(&expected, PRESET_2);

    g_assert_true(gst_value_compare(&expected, &result) == GST_VALUE_EQUAL);

    g_value_unset(&expected);
    g_value_unset(&result);
    gst_object_unref(element);
    gst_deinit();

    return 0;
}
