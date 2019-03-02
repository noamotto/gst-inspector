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

gchar ** gst_test_element_get_preset_names(GstPreset *preset)
{
    gchar **presets = g_new0(gchar *, 1);
    
    (void)preset;
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

    gst_init(&argc, &argv);
    gst_element_register(NULL, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    gst_inspector_inspect_element_presets(element, &result);

    g_assert_false(G_IS_VALUE(&result));

    gst_object_unref(element);
    gst_deinit();

    return 0;
}
