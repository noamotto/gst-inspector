#include "gstinspector.h"

static void fake_inspector(GstPlugin *plugin, GValue *result)
{
    (void)plugin;
    (void)result;
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    gst_inspector_clear_plugin_inspectors();
    g_assert_false(gst_inspector_remove_plugin_inspector("test"));

    return 0;
}
