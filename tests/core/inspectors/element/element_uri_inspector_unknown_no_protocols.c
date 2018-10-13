#include "gstinspectors.h"
#include "testutils.h"

#define FACTORY_NAME ("testelement")

static gboolean cheat_flag = FALSE;

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

static GstURIType get_type(GType self)
{
    (void)self;
    return cheat_flag ? GST_URI_UNKNOWN : GST_URI_SRC;
}

static const gchar *const *get_protocols(GType self)
{
    static const gchar * protocols[] = {NULL};
    (void)self;
    return protocols;
}

static void uri_init(GstURIHandlerInterface *iface)
{
    iface->get_type = get_type;
    iface->get_protocols = get_protocols;
}

G_DEFINE_TYPE_WITH_CODE(GstTestElement, gst_test_element, GST_TYPE_ELEMENT,
                        G_IMPLEMENT_INTERFACE(GST_TYPE_URI_HANDLER, uri_init))

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
    const GstStructure *uri_data;

    gst_init(&argc, &argv);
    gst_element_register(NULL, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    cheat_flag = TRUE;
    gst_inspector_inspect_element_uri_handler(element, &result);

    g_assert_true(GST_VALUE_HOLDS_STRUCTURE(&result));
    uri_data = gst_value_get_structure(&result);

    g_assert_true(gst_structure_has_field_typed(uri_data, "URI handler type",
                                                G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(uri_data, "URI handler type"),
                    ==, "unknown");
                    
    g_assert_true(gst_structure_has_field_typed(uri_data, "Supported URI protocols",
                                                G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(uri_data, "Supported URI protocols"),
                    ==, "none");

    g_value_unset(&result);
    gst_object_unref(element);

    return 0;
}
