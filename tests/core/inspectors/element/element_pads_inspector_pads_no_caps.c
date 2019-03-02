#include "gstinspectors.h"
#include "testutils.h"

#define FACTORY_NAME ("testelement")
static GstPad *src, *sink, *unknown;

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

static void gst_test_element_finalize(GstTestElement *self) 
{
    gst_element_remove_pad(GST_ELEMENT(self), src);
    gst_element_remove_pad(GST_ELEMENT(self), sink);
    gst_element_remove_pad(GST_ELEMENT(self), unknown);
    G_OBJECT_CLASS(gst_test_element_parent_class)->finalize(G_OBJECT(self));
}

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
    src = gst_pad_new("src", GST_PAD_SRC);
    gst_element_add_pad(GST_ELEMENT(self), src);

    sink = gst_pad_new("sink", GST_PAD_SINK);
    gst_element_add_pad(GST_ELEMENT(self), sink);

    unknown = gst_pad_new("unknown", GST_PAD_SRC);
    gst_element_add_pad(GST_ELEMENT(self), unknown);
    GST_PAD_DIRECTION(unknown) = GST_PAD_UNKNOWN;
}

int main(int argc, char *argv[])
{
    GstElement *element;
    GValue result = G_VALUE_INIT;
    const GstStructure *expected_pad;

    gst_init(&argc, &argv);
    gst_element_register(NULL, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    gst_inspector_inspect_element_pads(element, &result);

    g_assert_cmpint(gst_array_get_size(&result), ==, 3);

    expected_pad = gst_value_get_structure(gst_array_get_value(&result, 0));
    g_assert_true(gst_structure_has_field_typed(expected_pad, "Type", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(expected_pad, "Type"), ==, "SRC");
    g_assert_false(gst_structure_has_field(expected_pad, "Pad Template"));
    g_assert_false(gst_structure_has_field(expected_pad, "Capabilities"));

    expected_pad = gst_value_get_structure(gst_array_get_value(&result, 1));
    g_assert_true(gst_structure_has_field_typed(expected_pad, "Type", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(expected_pad, "Type"), ==, "SINK");
    g_assert_false(gst_structure_has_field(expected_pad, "Pad Template"));
    g_assert_false(gst_structure_has_field(expected_pad, "Capabilities"));

    expected_pad = gst_value_get_structure(gst_array_get_value(&result, 2));
    g_assert_true(gst_structure_has_field_typed(expected_pad, "Type", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(expected_pad, "Type"), ==, "UNKNOWN");
    g_assert_false(gst_structure_has_field(expected_pad, "Pad Template"));
    g_assert_false(gst_structure_has_field(expected_pad, "Capabilities"));

    g_value_unset(&result);
    gst_object_unref(element);

    return 0;
}
