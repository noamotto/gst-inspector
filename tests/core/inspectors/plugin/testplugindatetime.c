#define PACKAGE "none"
#define GST_PACKAGE_RELEASE_DATETIME "2018-10-05T12:32Z"

#include <gst/gst.h>

static gboolean plugin_init(GstPlugin *plugin)
{
    (void)plugin;
    return TRUE;
}

GST_PLUGIN_DEFINE(GST_VERSION_MAJOR,
                  GST_VERSION_MINOR,
                  testplugindatetime,
                  "Test Plugin",
                  plugin_init,
                  "0.0.1",
                  GST_LICENSE_UNKNOWN,
                  "test",
                  "here")
