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
    g_assert_true(gst_inspector_register_plugin_inspector(&fake_inspector, "test", "test", 0));

    gst_deinit();

    return 0;
}
