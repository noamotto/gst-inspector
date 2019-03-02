#include "gstinspectors.h"
#include "testutils.h"

#define FACTORY_NAME ("testelement")
#define CHILD_NAME ("testchild")

typedef struct _GstTestChild
{
    GstElement element;
} GstTestChild;

typedef struct _GstTestChildClass
{
    GstElementClass klass;
} GstTestChildClass;

#define GST_TYPE_TEST_CHILD gst_test_child_get_type()
GType gst_test_child_get_type(void);

G_DEFINE_TYPE(GstTestChild, gst_test_child, GST_TYPE_ELEMENT)

void gst_test_child_class_init(GstTestChildClass *klass)
{
    gst_element_class_set_static_metadata(GST_ELEMENT_CLASS(klass),
                                          CHILD_NAME,
                                          "TEST",
                                          "Test Element",
                                          "Noam Ottolenghi");
}

void gst_test_child_init(GstTestChild *self)
{
    (void)self;
}

typedef struct _GstTestElement
{
    GstBin element;
} GstTestElement;

typedef struct _GstTestElementClass
{
    GstBinClass klass;
} GstTestElementClass;


#define GST_TYPE_TEST_ELEMENT gst_test_element_get_type()
GType gst_test_element_get_type(void);

G_DEFINE_TYPE(GstTestElement, gst_test_element, GST_TYPE_BIN)

void gst_test_element_class_init(GstTestElementClass *klass)
{
    gst_element_class_set_static_metadata(GST_ELEMENT_CLASS(klass),
                                          FACTORY_NAME,
                                          "TEST",
                                          "Test Element",
                                          "Noam Ottolenghi");

    gst_element_register(NULL, CHILD_NAME, GST_RANK_NONE, GST_TYPE_TEST_CHILD);
}

#define CHILD1 "elem1"
#define CHILD2 "elem1"

void gst_test_element_init(GstTestElement *self)
{
    gst_bin_add(GST_BIN(self), gst_element_factory_make(CHILD_NAME, CHILD1));
    gst_bin_add(GST_BIN(self), gst_element_factory_make(CHILD_NAME, CHILD2));
}

int main(int argc, char *argv[])
{
    GstElement *element;
    GValue result = G_VALUE_INIT;
    GValue expected = G_VALUE_INIT;

    gst_init(&argc, &argv);
    gst_element_register(NULL, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    gst_inspector_inspect_element_children(element, &result);

    g_assert_true(GST_VALUE_HOLDS_LIST(&result));
    g_value_init(&expected, GST_TYPE_LIST);
    gst_array_append_static_string(&expected, CHILD1);
    gst_array_append_static_string(&expected, CHILD2);
    g_assert_true(gst_value_compare(&result, &expected));
    
    g_value_unset(&expected);
    g_value_unset(&result);
    gst_object_unref(element);
    gst_deinit();

    return 0;
}
