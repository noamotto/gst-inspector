#include "gstinspectors.h"
#include "testutils.h"

#define FACTORY_NAME ("testelement")

typedef struct _GstIface1Interface
{
    GTypeInterface iface;
} GstIface1Interface;

#define GST_TYPE_IFACE_1 gst_iface_1_get_type()
GType gst_iface_1_get_type(void);
G_DEFINE_INTERFACE(GstIface1, gst_iface_1, 0)

typedef struct _GstIface2Interface
{
    GTypeInterface iface;
} GstIface2Interface;

#define GST_TYPE_IFACE_2 gst_iface_2_get_type()
GType gst_iface_2_get_type(void);
G_DEFINE_INTERFACE(GstIface2, gst_iface_2, 0)

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

void gst_iface_1_default_init(GstIface1Interface *iface)
{
    (void)iface;
}

void gst_iface_2_default_init(GstIface2Interface *iface)
{
    (void)iface;
}

static void init_iface1(GstIface1Interface *iface)
{
    (void)iface;
}

static void init_iface2(GstIface2Interface *iface)
{
    (void)iface;
}

G_DEFINE_TYPE_WITH_CODE(GstTestElement, gst_test_element, GST_TYPE_ELEMENT,
                        G_IMPLEMENT_INTERFACE(GST_TYPE_IFACE_1, init_iface1);
                        G_IMPLEMENT_INTERFACE(GST_TYPE_IFACE_2, init_iface2);)

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

int main(int argc, char *argv[])
{
    GstElement *element;
    GValue result = G_VALUE_INIT;
    GValue expected_ifaces = G_VALUE_INIT;

    gst_init(&argc, &argv);
    gst_element_register(NULL, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    gst_inspector_inspect_element_interfaces(element, &result);

    g_assert_true(GST_VALUE_HOLDS_LIST(&result));

    g_value_init(&expected_ifaces, GST_TYPE_LIST);
    gst_array_append_static_string(&expected_ifaces, "GstIface1");
    gst_array_append_static_string(&expected_ifaces, "GstIface2");
    g_assert_true(gst_value_compare(&expected_ifaces, &result) == GST_VALUE_EQUAL);
    g_value_unset(&expected_ifaces);

    g_value_unset(&result);
    gst_object_unref(element);

    return 0;
}
