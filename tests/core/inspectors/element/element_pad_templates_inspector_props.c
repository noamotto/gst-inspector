#include "gstinspectors.h"
#include "testutils.h"

#define FACTORY_NAME ("testelement")
#define TEST_PROP_ID (1)

typedef struct _GstTestElement
{
    GstElement element;
} GstTestElement;

typedef struct _GstTestElementClass
{
    GstElementClass klass;
} GstTestElementClass;

static GstStaticPadTemplate template =
    GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC,
                            GST_PAD_ALWAYS, GST_STATIC_CAPS_ANY);

#define GST_TYPE_TEST_ELEMENT gst_test_element_get_type()
GType gst_test_element_get_type(void);

G_DEFINE_TYPE(GstTestElement, gst_test_element, GST_TYPE_ELEMENT)

typedef struct _GstTestPad
{
    GstPad pad;
} GstTestPad;

typedef struct _GstTestPadClass
{
    GstPadClass klass;
} GstTestPadClass;

#define GST_TYPE_TEST_PAD gst_test_pad_get_type()
GType gst_test_pad_get_type(void);

G_DEFINE_TYPE(GstTestPad, gst_test_pad, GST_TYPE_PAD)

void pad_get_prop(GObject *object,
                  guint property_id,
                  GValue *value,
                  GParamSpec *pspec)
{
    switch (property_id)
    {
    case TEST_PROP_ID:
    {
        g_value_set_boolean(value, TRUE);
        break;
    }
    default:
    {
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
    }
}

void pad_set_prop(GObject *object,
                  guint property_id,
                  const GValue *value,
                  GParamSpec *pspec)
{
    switch (property_id)
    {
    case TEST_PROP_ID:
    {
        (void)value;
        break;
    }
    default:
    {
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
    }
}

void gst_test_pad_class_init(GstTestPadClass *klass)
{
    G_OBJECT_CLASS(klass)->get_property = pad_get_prop;
    G_OBJECT_CLASS(klass)->set_property = pad_set_prop;
    g_object_class_install_property(G_OBJECT_CLASS(klass), TEST_PROP_ID,
                                    g_param_spec_boolean("param", "param",
                                                         "param", TRUE, G_PARAM_READWRITE));
}

void gst_test_pad_init(GstTestPad *self)
{
    (void)self;
}

void gst_test_element_class_init(GstTestElementClass *klass)
{
    gst_element_class_set_static_metadata(GST_ELEMENT_CLASS(klass),
                                          "testelement",
                                          "TEST",
                                          "Test Element",
                                          "Noam Ottolenghi");

    gst_element_class_add_static_pad_template_with_gtype(GST_ELEMENT_CLASS(klass),
                                                         &template, GST_TYPE_TEST_PAD);
}

void gst_test_element_init(GstTestElement *self)
{
    (void)self;
}

int main(int argc, char *argv[])
{
    GstElement *element;
    GValue result = G_VALUE_INIT;
    const GstStructure *pad_data;
    const GstStructure *params_data, *param_data;

    gst_init(&argc, &argv);
    gst_element_register(NULL, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    gst_inspector_inspect_pad_templates(element, &result);

    g_assert_true(GST_VALUE_HOLDS_ARRAY(&result));
    g_assert_true(gst_value_array_get_size(&result) == 1);

    pad_data = gst_value_get_structure(gst_value_array_get_value(&result, 0));
    check_pad_template(&template, pad_data);

    g_assert_true(gst_structure_has_field_typed(pad_data, "Pad Properties",
                                                GST_TYPE_STRUCTURE));
    params_data = gst_dictionary_get_sub_dictionary(pad_data, "Pad Properties");

    g_assert_true(gst_structure_has_field_typed(params_data, "param",
                                                GST_TYPE_STRUCTURE));
    param_data = gst_dictionary_get_sub_dictionary(params_data, "param");

    g_assert_true(gst_structure_has_field_typed(param_data, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(param_data, KEY_NAME), ==, "param: param");

    g_assert_true(gst_structure_has_field_typed(param_data, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(param_data, KEY_TYPE), ==, "Boolean");

    g_assert_true(gst_structure_has_field_typed(param_data, KEY_VALUE, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(param_data, KEY_VALUE), ==, "true");

    g_value_unset(&result);
    gst_object_unref(element);

    return 0;
}
