#include "gstinspectors.h"
#include "gstinspector_priv.h"

void gst_inspector_inspect_element_children(GstElement *element, GValue *result)
{
    g_return_if_fail(GST_IS_ELEMENT(element));

    if (GST_IS_BIN(element))
    {
        GList *children = GST_BIN(element)->children;

        if (children)
        {
            g_value_init(result, GST_TYPE_LIST);

            while (children)
            {
                gst_array_append_static_string(result,
                                               GST_ELEMENT_NAME(GST_ELEMENT(children->data)));
                children = g_list_next(children);
            }
        }
    }
}
