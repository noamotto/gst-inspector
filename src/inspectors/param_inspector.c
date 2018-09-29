#include "gstinspectors.h"
#include "type_reader/type_reader.h"

static gint cmp_pspecs(GParamSpec **p1, GParamSpec **p2)
{
    return g_strcmp0(g_param_spec_get_name(*p1), g_param_spec_get_name(*p2));
}

GstStructure *param_inspector(GstElement *element)
{
    GstStructure *dictionary;
    guint n_properties;
    GParamSpec **pspecs;

    g_return_val_if_fail(GST_IS_ELEMENT(element), NULL);

    pspecs = g_object_class_list_properties(G_OBJECT_GET_CLASS(G_OBJECT(element)),
                                            &n_properties);
    g_qsort_with_data(pspecs, (gint)n_properties, sizeof(GParamSpec *),
                      (GCompareDataFunc)cmp_pspecs, NULL);

    dictionary = gst_structure_new_empty("params");

    for (guint i = 0; i < n_properties; i++)
    {
        GstStructure *param_dict = gst_structure_new_empty(pspecs[i]->name);
        GValue param_val = G_VALUE_INIT;
        g_value_init(&param_val, pspecs[i]->value_type);

        if (pspecs[i]->flags & G_PARAM_READABLE)
        {
            g_object_get_property(G_OBJECT(element), pspecs[i]->name, &param_val);
        }
        else
        {
            g_param_value_set_default(pspecs[i], &param_val);
        }

        gst_type_reader_fill_type(pspecs[i], &param_val, param_dict);
        gst_dictionary_set_sub_dictionary(dictionary, pspecs[i]->name, param_dict);

        g_value_unset(&param_val);
    }

    return dictionary;
}
