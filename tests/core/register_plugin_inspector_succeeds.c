#include "gstinspector.h"

static GstStructure *fake_inspector(GstPlugin *plugin)
{
    (void)plugin;
    return NULL;
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    g_assert_true(gst_inspector_register_plugin_inspector(&fake_inspector, "test", "test", 0));

    return 0;
}
