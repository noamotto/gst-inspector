#include "gstinspector.h"

#define DESCRIPTION_KEY ("Error")

int main(int argc, char *argv[])
{
    GstStructure *data;
    const gchar *test_field_value;
    gchar *error_message;

    gst_init(&argc, &argv);

    error_message = gst_info_strdup_printf(
        "%" GST_PTR_FORMAT " is not a valid GstPluginFeature", NULL);
    data = gst_inspector_inspect_plugin_feature(NULL);

    test_field_value = gst_dictionary_get_string(data, DESCRIPTION_KEY);
    g_assert_nonnull(test_field_value);
    g_assert_cmpstr(test_field_value, ==, error_message);

    gst_structure_free(data);
    g_free(error_message);
    gst_deinit();

    return 0;
}
