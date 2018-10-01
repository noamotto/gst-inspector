#include "gstinspectors.h"
#include "gstinspector_priv.h"

void gst_inspector_inspect_element_interfaces(GstElement *element, GValue *result)
{
    GArray *ifaces;

    g_return_if_fail(GST_IS_ELEMENT(element));

    ifaces = parse_type_interfaces(G_OBJECT_TYPE(element));

    if (ifaces)
    {
        g_value_init(result, G_TYPE_ARRAY);
        g_value_take_boxed(result, ifaces);
    }
}
