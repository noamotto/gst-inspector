#include "gstinspector.h"

static void fake_inspector(GstPlugin *plugin, GValue *result)
{
    (void)plugin;
    (void)result;
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    g_assert_true(gst_inspector_register_plugin_inspector(fake_inspector, "test", "test", 0));
    g_assert_false(gst_inspector_register_plugin_inspector(fake_inspector, "test", "test", 0));

    return 0;
}
