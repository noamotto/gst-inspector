/*
 *  Element actions inspector implementation
 */
#include "gstinspectors.h"
#include "gstinspector_priv.h"

/** 
 *  @addtogroup element-inspectors 
 *  @{ 
 */

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

/**
 *  @brief Inspects element action signals
 * 
 *  Inspects the action signals of a single element
 * 
 *  @param element Element to inspect
 *  @param result 
 *  @parblock
 *  The inspected data
 * 
 *  The inspected data is an array of signals, if found. Each signal consists of
 *  these fields:
 *  - <b>Signal name</b> - Name of signal
 *  - <b>Return type</b> - Signal function's return type
 *  - <b>Signal parameters</b> - Signal function's parameters
 *  @endparblock
 */
void gst_inspector_inspect_element_actions(GstElement *element, GValue *result)
{
    g_return_if_fail(GST_IS_ELEMENT(element));

    GSList *found_actions = NULL;

    found_actions = populate_object_actions(element, found_actions);

    if (found_actions)
    {
        GSList *iterator = found_actions;
        g_value_init(result, GST_TYPE_LIST);

        while (iterator)
        {
            GSignalQuery *query = (GSignalQuery *)iterator->data;

            gst_array_append_subdictionary(result, parse_signal(query));

            iterator = iterator->next;
        }

        g_slist_foreach(found_actions, (GFunc)g_free, NULL);
        g_slist_free(found_actions);
    }
}

/** @}*/
