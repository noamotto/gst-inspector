#include "gstinspector.h"
#include "gsttestplugin.h"

#define DESCRIPTION_KEY ("Error")

int main(int argc, char *argv[])
{
    GstStructure *data;
    gchar *expected_message;
    const gchar *actual_message;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    expected_message = gst_info_strdup_printf(
        "%" GST_PTR_FORMAT " is not a valid GstPlugin", NULL);

    data = gst_inspector_inspect_plugin(NULL);

    actual_message = gst_dictionary_get_string(data, DESCRIPTION_KEY);
    g_assert_cmpstr(actual_message, ==, expected_message);

    gst_structure_free(data);
    g_free(expected_message);
    return 0;
}
