#include "gstinspectors.h"
#include "gstinspector_priv.h"

void gst_inspector_inspect_element_hierarchy(GstElement *element, GValue *result)
{
    g_return_if_fail(GST_IS_ELEMENT(element));

    g_value_init(result, G_TYPE_ARRAY);
    g_value_take_boxed(result, parse_type_hierarchy(G_OBJECT_TYPE(element)));
}
