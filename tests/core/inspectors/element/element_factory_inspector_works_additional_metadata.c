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

    gst_element_class_add_metadata(GST_ELEMENT_CLASS(klass), "test", "test");
}

void gst_test_element_init(GstTestElement *self)
{
    (void)self;
}

int main(int argc, char *argv[])
{
    GstElement *element;
    GstElementFactory *factory;
    GValue result = G_VALUE_INIT;
    GstStructure *data;
    guint rank;
    gchar *rank_str;

    gst_init(&argc, &argv);

    gst_element_register(NULL, FACTORY_NAME, GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);

    element = gst_element_factory_make(FACTORY_NAME, NULL);
    factory = gst_element_get_factory(element);
    gst_inspector_inspect_factory_details(element, &result);

    g_assert_true(GST_VALUE_HOLDS_STRUCTURE(&result));
    data = (GstStructure *)g_value_get_boxed(&result);

    rank = gst_plugin_feature_get_rank(GST_PLUGIN_FEATURE(factory));
    rank_str = get_rank_name((gint)rank);
    g_assert_true(gst_structure_has_field_typed(data, "Rank", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Rank"), ==, rank_str);
    g_free(rank_str);

    g_assert_true(gst_structure_has_field_typed(data, "Long-name", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Long-name"), ==,
                    gst_element_class_get_metadata(GST_ELEMENT_GET_CLASS(element),
                                                   GST_ELEMENT_METADATA_LONGNAME));

    g_assert_true(gst_structure_has_field_typed(data, "Klass", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Klass"), ==,
                    gst_element_class_get_metadata(GST_ELEMENT_GET_CLASS(element),
                                                   GST_ELEMENT_METADATA_KLASS));

    g_assert_true(gst_structure_has_field_typed(data, "Description", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Description"), ==,
                    gst_element_class_get_metadata(GST_ELEMENT_GET_CLASS(element),
                                                   GST_ELEMENT_METADATA_DESCRIPTION));

    g_assert_true(gst_structure_has_field_typed(data, "Author", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Author"), ==,
                    gst_element_class_get_metadata(GST_ELEMENT_GET_CLASS(element),
                                                   GST_ELEMENT_METADATA_AUTHOR));

    g_assert_true(gst_structure_has_field_typed(data, "Test", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(data, "Test"), ==,
                    gst_element_class_get_metadata(GST_ELEMENT_GET_CLASS(element),
                                                   "test"));
    g_value_unset(&result);
    gst_object_unref(element);
    gst_deinit();

    return 0;
}
