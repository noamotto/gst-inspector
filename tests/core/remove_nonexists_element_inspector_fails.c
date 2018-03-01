#include "gstinspector.h"

static GstStructure *fake_inspector(GstElementFactory *element)
{
    return NULL;
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    g_assert_false(gst_inspector_remove_element_inspector("test"));

    return 0;
}
