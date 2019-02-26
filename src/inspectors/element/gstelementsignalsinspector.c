#include "gstinspectors.h"
#include "gstinspector_priv.h"

static GSList *populate_sometimes_template_signals(GstElement *element,
                                                   GSList *found_signals)
{
    GstElementClass *klass = GST_ELEMENT_GET_CLASS(element);

    for (GList *l = klass->padtemplates; l != NULL; l = l->next)
    {
        if (GST_PAD_TEMPLATE(l->data)->presence == GST_PAD_SOMETIMES)
        {
            GSignalQuery *query = g_new0(GSignalQuery, 1);
            g_signal_query(g_signal_lookup("pad-added", GST_TYPE_ELEMENT), query);
            found_signals = g_slist_append(found_signals, query);

            query = g_new0(GSignalQuery, 1);
            g_signal_query(g_signal_lookup("pad-removed", GST_TYPE_ELEMENT), query);
            found_signals = g_slist_append(found_signals, query);

            query = g_new0(GSignalQuery, 1);
            g_signal_query(g_signal_lookup("no-more-pads", GST_TYPE_ELEMENT), query);
            found_signals = g_slist_append(found_signals, query);
        }
    }

    return found_signals;
}

static GSList *populate_object_signals(GstElement *element, GSList *found_signals)
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

            if (!(query->signal_flags & G_SIGNAL_ACTION))
            {
                found_signals = g_slist_append(found_signals, query);
            }
            else
            {
                g_free(query);
            }
        }
        g_free(signals);
    }

    return found_signals;
}

void gst_inspector_inspect_element_signals(GstElement *element, GValue *result)
{
    g_return_if_fail(GST_IS_ELEMENT(element));

    GSList *found_signals = NULL;

    found_signals = populate_sometimes_template_signals(element, found_signals);
    found_signals = populate_object_signals(element, found_signals);

    if (found_signals)
    {
        GSList *iterator = found_signals;
        g_value_init(result, GST_TYPE_LIST);

        while (iterator)
        {
            GSignalQuery *query = (GSignalQuery *)iterator->data;

            gst_array_append_subdictionary(result, parse_signal(query));

            iterator = iterator->next;
        }

        g_slist_foreach(found_signals, (GFunc)g_free, NULL);
        g_slist_free(found_signals);
    }
}
