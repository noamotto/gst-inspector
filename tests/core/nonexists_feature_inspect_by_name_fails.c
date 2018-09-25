#include "gstinspector.h"
#include "gsttestplugin.h"

#define FAKE_NAME ("sdfgfnyurcdfbgtyhdvzdbssdfgbnxfmuyww") // Absurd fake name. Hopefully won't be used in the future
#define ELEMENT_TEST_NAME ("ElementTest")
#define PLUGIN_TEST_NAME ("PluginTest")
#define TEST_FIELD_NAME ("Test field")
#define TEST_FIELD_VALUE ("hello")

#define ERROR_MESSAGE_NAME ("Error")
#define DESCRIPTION_KEY ("Description")

static GstStructure *fake_element_inspector(GstElement *element)
{
    (void)element;
    GstStructure *test = gst_structure_new_empty(ELEMENT_TEST_NAME);
    gst_dictionary_set_string(test, TEST_FIELD_NAME, TEST_FIELD_VALUE);
    return test;
}

static GstStructure *fake_plugin_inspector(GstPlugin *plugin)
{
    (void)plugin;
    GstStructure *test = gst_structure_new_empty(PLUGIN_TEST_NAME);
    gst_dictionary_set_string(test, TEST_FIELD_NAME, TEST_FIELD_VALUE);
    return test;
}

int main(int argc, char *argv[])
{
    GstStructure *data;
    gchar *expected_message = g_strdup_printf("Could not find object named %s", FAKE_NAME);
    const gchar *actual_message;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    gst_inspector_register_element_inspector(fake_element_inspector, ELEMENT_TEST_NAME, -1);
    gst_inspector_register_plugin_inspector(fake_plugin_inspector, PLUGIN_TEST_NAME, -1);

    data = gst_inspector_inspect_by_name(FAKE_NAME);

    g_assert_cmpstr(ERROR_MESSAGE_NAME, ==, gst_structure_get_name(data));


    actual_message = gst_dictionary_get_string(data, DESCRIPTION_KEY);
    g_assert_cmpstr(actual_message, ==, expected_message);

    return 0;
}
