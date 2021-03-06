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
    GST_OBJECT_FLAG_SET(self, GST_ELEMENT_FLAG_REQUIRE_CLOCK);
    GST_OBJECT_FLAG_SET(self, GST_ELEMENT_FLAG_PROVIDE_CLOCK);
}

int main(int argc, char *argv[])
{
    GstElement *element;
    GValue result = G_VALUE_INIT;
    GValue expected = G_VALUE_INIT;
    GstClock *clock;

    gst_init(&argc, &argv);
    gst_element_register(NULL, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    gst_inspector_inspect_element_clocking(element, &result);

    g_value_init(&expected, GST_TYPE_LIST);
    clock = gst_element_get_clock(element);
    gst_array_append_static_string(&expected, "element requires a clock");
    gst_array_append_static_string(&expected,
                                   "element is supposed to provide a clock but returned NULL");

    g_assert_true(gst_value_compare(&result, &expected) == GST_VALUE_EQUAL);

    g_value_unset(&expected);
    g_value_unset(&result);
    gst_object_unref(element);
    gst_deinit();

    return 0;
}
