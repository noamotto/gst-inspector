#include "gstinspector.h"
#include "gsttestplugin.h"

#define TEST_NAME ("test")
#define TEST_FIELD_NAME ("Test field")
#define TEST_FIELD_VALUE ("hello")

#define ERROR_MESSAGE_NAME ("Error")
#define DESCRIPTION_KEY ("Description")

static GstStructure *fake_inspector(GstElement *element)
{
    (void)element;
    GstStructure *test = gst_structure_new_empty(TEST_NAME);
    gst_dictionary_set_string(test, TEST_FIELD_NAME, TEST_FIELD_VALUE);
    return test;
}

int main(int argc, char *argv[])
{
    GstStructure *data;
    gchar *expected_message;
    const gchar *actual_message;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    expected_message = gst_info_strdup_printf(
        "%" GST_PTR_FORMAT " is not a valid GstElementFactory", NULL);

    gst_inspector_register_element_inspector(fake_inspector, TEST_NAME, -1);

    data = gst_inspector_inspect_element(NULL);

    g_assert_cmpstr(ERROR_MESSAGE_NAME, ==, gst_structure_get_name(data));

    actual_message = gst_dictionary_get_string(data, DESCRIPTION_KEY);
    g_assert_cmpstr(actual_message, ==, expected_message);

    return 0;
}
