#include "gstinspector.h"

static gchar *test_list[] = {"test1", "test2", "test3"};

static GstStructure *fake_inspector(GstElement *element)
{
    (void)element;
    return NULL;
}

int main(int argc, char *argv[])
{
    gchar ** names = NULL;

    gst_init(&argc, &argv);

    gst_inspector_register_element_inspector(fake_inspector, test_list[0], -1);
    gst_inspector_register_element_inspector(fake_inspector, test_list[1], -1);
    gst_inspector_register_element_inspector(fake_inspector, test_list[2], -1);

    names = gst_inspector_get_installed_element_inspectors();
    for (gsize i=0;i<3;i++)
    {
        g_assert_cmpstr(names[i], ==, test_list[i]);
    }

    g_free(names);

    return 0;
}
