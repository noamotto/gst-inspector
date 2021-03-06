#include "gstinspector.h"
#include "gsttestplugin.h"

#define TEST_NAME ("test")
#define TEST_FIELD_NAME ("Test field")
#define TEST_FIELD_VALUE ("hello")

#define DESCRIPTION_KEY ("Error")

static void fake_inspector(GstElement *element, GValue *result)
{
    (void)element;

    GstStructure *test = gst_structure_new_empty(TEST_NAME);
    gst_dictionary_set_string(test, TEST_FIELD_NAME, TEST_FIELD_VALUE);
    g_value_init(result, GST_TYPE_STRUCTURE);
    g_value_take_boxed(result, test);
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

    gst_inspector_clear_element_inspectors();
    gst_inspector_register_element_inspector(fake_inspector, TEST_NAME, TEST_NAME, -1);

    data = gst_inspector_inspect_element(NULL);

    actual_message = gst_dictionary_get_string(data, DESCRIPTION_KEY);
    g_assert_cmpstr(actual_message, ==, expected_message);

    g_free(expected_message);
    gst_structure_free(data);
    gst_deinit();

    return 0;
}
