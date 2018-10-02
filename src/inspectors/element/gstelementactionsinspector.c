#include "gstinspectors.h"
#include "gstinspector_priv.h"

static GSList *populate_object_actions(GstElement *element, GSList *found_actions)
{
    GSignalQuery *query = NULL;
    guint *signals;
    guint nsignals;

    for (GType type = G_OBJECT_TYPE(element); type; type = g_type_parent(type))
    {
        if (type == GST_TYPE_ELEMENT || type == GST_TYPE_OBJECT)
        {
            break;
        }
        if (type == GST_TYPE_BIN && G_OBJECT_TYPE(element) != GST_TYPE_BIN)
        {
            continue;
        }

        signals = g_signal_list_ids(type, &nsignals);
        for (guint i = 0; i < nsignals; i++)
        {
            query = g_new0(GSignalQuery, 1);
            g_signal_query(signals[i], query);

            if (query->signal_flags & G_SIGNAL_ACTION)
            {
                found_actions = g_slist_append(found_actions, query);
            }
            else
            {
                g_free(query);
            }
        }
        g_free(signals);
    }

    return found_actions;
}

void gst_inspector_inspect_element_actions(GstElement *element, GValue *result)
{
    g_return_if_fail(GST_IS_ELEMENT(element));

    GSList *found_actions = NULL;

    found_actions = populate_object_actions(element, found_actions);

    if (found_actions)
    {
        GSList *iterator = found_actions;
        GArray *signal_array = g_array_new(FALSE, FALSE, sizeof(GValue));
        g_array_set_clear_func(signal_array, (GDestroyNotify)g_value_unset);
        g_value_init(result, G_TYPE_ARRAY);

        while (iterator)
        {
            GSignalQuery *query = (GSignalQuery *)iterator->data;

            g_array_add_subdictionary(signal_array, parse_signal(query));

            iterator = iterator->next;
        }

        g_slist_foreach(found_actions, (GFunc)g_free, NULL);
        g_slist_free(found_actions);
        g_value_take_boxed(result, signal_array);
    }
}
