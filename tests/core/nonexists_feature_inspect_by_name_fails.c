#include "gstinspector.h"
#include "gsttestplugin.h"

#define FAKE_NAME ("sdfgfnyurcdfbgtyhdvzdbssdfgbnxfmuyww") // Absurd fake name. Hopefully won't be used in the future
#define ELEMENT_TEST_NAME ("ElementTest")
#define PLUGIN_TEST_NAME ("PluginTest")
#define TEST_FIELD_NAME ("Test field")
#define TEST_FIELD_VALUE ("hello")

#define DESCRIPTION_KEY ("Error")

static void fake_element_inspector(GstElement *element, GValue *result)
{
    (void)element;

    GstStructure *test = gst_structure_new_empty(ELEMENT_TEST_NAME);
    gst_dictionary_set_string(test, TEST_FIELD_NAME, TEST_FIELD_VALUE);
    g_value_init(result, GST_TYPE_STRUCTURE);
    g_value_take_boxed(result, test);
}

static void fake_plugin_inspector(GstPlugin *plugin, GValue *result)
{
    (void)plugin;

    GstStructure *test = gst_structure_new_empty(PLUGIN_TEST_NAME);
    gst_dictionary_set_string(test, TEST_FIELD_NAME, TEST_FIELD_VALUE);
    g_value_init(result, GST_TYPE_STRUCTURE);
    g_value_take_boxed(result, test);
}

int main(int argc, char *argv[])
{
    GstStructure *data;
    gchar *expected_message = g_strdup_printf("Could not find object named %s", FAKE_NAME);
    const gchar *actual_message;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    gst_inspector_clear_element_inspectors();
    gst_inspector_clear_plugin_inspectors();
    gst_inspector_register_element_inspector(fake_element_inspector, ELEMENT_TEST_NAME, ELEMENT_TEST_NAME, -1);
    gst_inspector_register_plugin_inspector(fake_plugin_inspector, PLUGIN_TEST_NAME, PLUGIN_TEST_NAME, -1);

    data = gst_inspector_inspect_by_name(FAKE_NAME);

    actual_message = gst_dictionary_get_string(data, DESCRIPTION_KEY);
    g_assert_cmpstr(actual_message, ==, expected_message);

    return 0;
}
