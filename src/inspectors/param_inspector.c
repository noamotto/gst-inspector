#include "gstinspectors.h"
#include "type_reader/type_reader.h"

GstStructure *param_inspector(GstElement *element)
{
    GstStructure *dictionary;
    guint n_properties;
    GParamSpec **pspecs;

    g_return_val_if_fail(GST_IS_ELEMENT(element), NULL);

    pspecs = g_object_class_list_properties(G_OBJECT_GET_CLASS(G_OBJECT(element)),
                                            &n_properties);
    dictionary = gst_structure_new_empty("Element Properties");

    for (guint i = 0; i < n_properties; i++)
    {
        GstStructure *param_dict = gst_structure_new_empty(pspecs[i]->name);
        GValue param_val;

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
