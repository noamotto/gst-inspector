#include "gstinspector.h"

static GstStructure *fake_inspector(GstElementFactory *element)
{
    (void)element;    
    return NULL;
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    g_assert_true(gst_inspector_register_element_inspector(fake_inspector, "test", 0));
    g_assert_false(gst_inspector_register_element_inspector(fake_inspector, "test", 0));

    return 0;
}
