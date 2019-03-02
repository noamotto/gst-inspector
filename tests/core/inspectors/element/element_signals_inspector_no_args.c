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

static guint signal_id;

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

    signal_id = g_signal_new("no-retval", GST_TYPE_TEST_ELEMENT, G_SIGNAL_RUN_LAST,
                             0, NULL, NULL, NULL, G_TYPE_NONE, 0);

    signal_id = g_signal_new("no-ptr", GST_TYPE_TEST_ELEMENT, G_SIGNAL_RUN_LAST,
                             0, NULL, NULL, NULL, G_TYPE_INT, 0);

    signal_id = g_signal_new("ptr", GST_TYPE_TEST_ELEMENT, G_SIGNAL_RUN_LAST,
                             0, NULL, NULL, NULL, GST_TYPE_ELEMENT, 0);
}

void gst_test_element_init(GstTestElement *self)
{
    (void)self;
}

static void check_signal(const GstStructure *expected_signal,
                         const gchar *expected_name,
                         const gchar *expected_rettype,
                         int expected_n_params,
                         const gchar **expected_params)
{
    GValue expected_args = G_VALUE_INIT;
    g_assert_true(gst_structure_has_field_typed(expected_signal, "Signal name",
                                                G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(expected_signal, "Signal name"),
                    ==, expected_name);

    g_assert_true(gst_structure_has_field_typed(expected_signal, "Return type",
                                                G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(expected_signal, "Return type"),
                    ==, expected_rettype);

    g_assert_true(gst_structure_has_field_typed(expected_signal, "Signal parameters",
                                                GST_TYPE_LIST));
    g_value_init(&expected_args, GST_TYPE_LIST);
    gst_array_append_static_string(&expected_args, "GstTestElement* object");
    for (int i = 0; i < expected_n_params; i++)
    {
        gst_array_append_static_string(&expected_args, expected_params[i]);
    }
    gst_array_append_static_string(&expected_args, "gpointer user_data");
    g_assert_true(gst_value_compare(
                      gst_dictionary_get_array(expected_signal, "Signal parameters"),
                      &expected_args) == GST_VALUE_EQUAL);

    g_value_unset(&expected_args);
}

int main(int argc, char *argv[])
{
    GstElement *element;
    GValue result = G_VALUE_INIT;

    gst_init(&argc, &argv);
    gst_element_register(NULL, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    gst_inspector_inspect_element_signals(element, &result);

    g_assert_true(GST_VALUE_HOLDS_LIST(&result));
    g_assert_cmpuint(gst_array_get_size(&result), ==, 3);
    check_signal(gst_value_get_structure(gst_array_get_value(&result, 0)),
                 "no-retval", g_type_name(G_TYPE_NONE), 0, NULL);
    check_signal(gst_value_get_structure(gst_array_get_value(&result, 1)),
                 "no-ptr", "gint", 0, NULL);
    check_signal(gst_value_get_structure(gst_array_get_value(&result, 2)),
                 "ptr", "GstElement *", 0, NULL);

    g_value_unset(&result);
    gst_object_unref(element);
    gst_deinit();

    return 0;
}
