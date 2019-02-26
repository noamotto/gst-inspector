#include "gstinspectors.h"
#include "testutils.h"

#define FACTORY_NAME ("testelement")

static GstPad *pad, *peer;
static GstStaticCaps caps = GST_STATIC_CAPS("video/x-raw, width=320, height=240");
static GstStaticPadTemplate src_factory =
    GST_STATIC_PAD_TEMPLATE("factory", GST_PAD_SRC, GST_PAD_ALWAYS,
                            GST_STATIC_CAPS("video/x-raw, width=320, height=240"));

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
    pad = gst_pad_new_from_static_template(&src_factory, "src");
    peer = gst_pad_new("peer", GST_PAD_SINK);
    gst_element_add_pad(GST_ELEMENT(self), pad);

    // Perform event pushng to set caps
    gst_pad_link(pad, peer);
    gst_element_set_state(GST_ELEMENT(self), GST_STATE_PAUSED);

    gst_pad_push_event(pad, gst_event_new_stream_start("0"));
    gst_pad_push_event(pad, gst_event_new_caps(gst_static_caps_get(&caps)));
}

int main(int argc, char *argv[])
{
    GstElement *element;
    GValue result = G_VALUE_INIT;
    GValue expected_caps = G_VALUE_INIT;
    const GstStructure *expected_pad;

    gst_init(&argc, &argv);
    gst_element_register(NULL, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    gst_inspector_inspect_element_pads(element, &result);

    g_assert_cmpint(gst_array_get_size(&result), ==, 1);

    expected_pad = gst_value_get_structure(gst_array_get_value(&result, 0));
    g_assert_true(gst_structure_has_field_typed(expected_pad, "Type", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(expected_pad, "Type"), ==, "SRC");
    g_assert_true(gst_structure_has_field_typed(expected_pad, "Pad Template", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(expected_pad, "Pad Template"),
                    ==, src_factory.name_template);

    g_assert_true(gst_structure_has_field_typed(expected_pad, "Capabilities",
                                                GST_TYPE_LIST));
    parse_caps(gst_pad_get_current_caps(pad), &expected_caps);
    g_assert_true(gst_value_compare(
                      gst_dictionary_get_array(expected_pad, "Capabilities"),
                      &expected_caps) == GST_VALUE_EQUAL);

    g_value_unset(&expected_caps);
    g_value_unset(&result);
    gst_object_unref(element);

    return 0;
}
