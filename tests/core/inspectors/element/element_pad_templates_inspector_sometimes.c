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

static GstStaticPadTemplate templates[] =
    {GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC,
                             GST_PAD_SOMETIMES, GST_STATIC_CAPS_ANY),
     GST_STATIC_PAD_TEMPLATE("sink", GST_PAD_SINK,
                             GST_PAD_SOMETIMES, GST_STATIC_CAPS_ANY),
     GST_STATIC_PAD_TEMPLATE("unknoun", GST_PAD_UNKNOWN,
                             GST_PAD_SOMETIMES, GST_STATIC_CAPS_ANY)};

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

    gst_element_class_add_static_pad_template(GST_ELEMENT_CLASS(klass), &templates[0]);
    gst_element_class_add_static_pad_template(GST_ELEMENT_CLASS(klass), &templates[1]);
    gst_element_class_add_static_pad_template(GST_ELEMENT_CLASS(klass), &templates[2]);
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
    gst_inspector_inspect_pad_templates(element, &result);

    g_assert_true(GST_VALUE_HOLDS_LIST(&result));

    for (guint i = 0; i < gst_array_get_size(&result); i++)
    {
        const GstStructure *pad_data =
            gst_value_get_structure(gst_array_get_value(&result, i));
        check_pad_template(&templates[i], pad_data);

        g_assert_false(gst_structure_has_field(pad_data, "Pad Properties"));
    }

    g_value_unset(&result);
    gst_object_unref(element);

    return 0;
}
