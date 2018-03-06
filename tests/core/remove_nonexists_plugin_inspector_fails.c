#include "gstinspector.h"

static GstStructure *fake_inspector(GstPlugin *plugin)
{
    (void)plugin;
    return NULL;
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    g_assert_false(gst_inspector_remove_plugin_inspector("test"));

    return 0;
}
