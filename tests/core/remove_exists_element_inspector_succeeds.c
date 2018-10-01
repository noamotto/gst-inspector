#include "gstinspector.h"

static void fake_inspector(GstElement *element, GValue *result)
{
    (void)element;
    (void)result;
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    g_assert_true(gst_inspector_register_element_inspector(fake_inspector, "test", "test", 0));
    g_assert_true(gst_inspector_remove_element_inspector("test"));

    return 0;
}
