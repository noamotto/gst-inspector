#include "gstinspector.h"
#include "gsttestplugin.h"

#define TYPEFIND_NAME   ("testtypefind")
#define TYPE_KEY        ("Type")
#define TYPE_VALUE      ("A typefind function")

int main(int argc, char *argv[])
{
    GstStructure *data;
    const gchar *test_field_value;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    data = gst_inspector_inspect_by_name(TYPEFIND_NAME);

    g_assert_cmpstr(TYPEFIND_NAME, ==, gst_structure_get_name(data));

    test_field_value = gst_dictionary_get_string(data, TYPE_KEY);
    g_assert_nonnull(test_field_value);
    g_assert_cmpstr(test_field_value, ==, TYPE_VALUE);

    return 0;
}
