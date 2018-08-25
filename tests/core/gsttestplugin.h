/**
 * Small plugin for test purposes
 */

#include <gst/gst.h>

#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

G_BEGIN_DECLS

typedef struct _GstTestElement
{
    GstElement element;
} GstTestElement;

typedef struct _GstTestElementClass
{
    GstElementClass element;
} GstTestElementClass;

#define GST_TYPE_TEST_ELEMENT gst_test_element_get_type()
GType gst_test_element_get_type(void);

// Test element. Does practically nothing, for test purposes
G_DEFINE_TYPE(GstTestElement, gst_test_element, GST_TYPE_ELEMENT)

void gst_test_element_class_init(GstTestElementClass *klass)
{
    gst_element_class_set_static_metadata(GST_ELEMENT_CLASS(klass),
                                          "testelement",
                                          "TEST",
                                          "Test Element",
                                          "Noam Ottolenghi");
}

void gst_test_element_init(GstTestElement *self)
{
    (void)self;
}

//Test plugin
static gboolean plugin_init(GstPlugin *plugin)
{
    return gst_element_register(plugin, "testelement", GST_RANK_NONE, GST_TYPE_TEST_ELEMENT);
}

GST_PLUGIN_STATIC_DECLARE(testplugin);

void gst_plugin_testplugin_register(void)
{
    gst_plugin_register_static(GST_VERSION_MAJOR,
                               GST_VERSION_MINOR,
                               "testplugin",
                               "Test Plugin",
                               plugin_init,
                               "0.0.1",
                               GST_LICENSE_UNKNOWN,
                               "none",
                               "test",
                               "here");
}

G_END_DECLS

#endif //TESTPLUGIN_H
