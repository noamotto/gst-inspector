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
            GArray *children_array = g_array_new(FALSE, FALSE, sizeof(GValue));
            g_array_set_clear_func(children_array, (GDestroyNotify)g_value_unset);
            g_value_init(result, G_TYPE_ARRAY);

            while (children)
            {
                g_array_add_static_string(children_array,
                                          GST_ELEMENT_NAME(GST_ELEMENT(children->data)));
                children = g_list_next(children);
            }

            g_value_take_boxed(result, children_array);
        }
    }
}
