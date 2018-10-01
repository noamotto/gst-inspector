#include "gstinspector.h"

static void fake_inspector(GstElement *element, GValue *result)
{
    (void)element;
    (void)result;
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    g_assert_false(gst_inspector_remove_element_inspector("test"));

    return 0;
}
