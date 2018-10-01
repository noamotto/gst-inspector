#include "gstinspectors.h"
#include "gstinspector_priv.h"
#include "type_reader/type_reader.h"

static gint cmp_pspecs(GParamSpec **p1, GParamSpec **p2)
{
    return g_strcmp0(g_param_spec_get_name(*p1), g_param_spec_get_name(*p2));
}

void gst_inspector_inspect_element_properties(GstElement *element, GValue *result)
{
    GstStructure *dictionary;
    guint n_properties;
    GParamSpec **pspecs;

    g_return_if_fail(GST_IS_ELEMENT(element));

    g_value_init(result, GST_TYPE_STRUCTURE);
    pspecs = g_object_class_list_properties(G_OBJECT_GET_CLASS(G_OBJECT(element)),
                                            &n_properties);
    g_qsort_with_data(pspecs, (gint)n_properties, sizeof(GParamSpec *),
                      (GCompareDataFunc)cmp_pspecs, NULL);

    dictionary = gst_structure_new_empty("params");

    for (guint i = 0; i < n_properties; i++)
    {
        gst_dictionary_set_sub_dictionary(dictionary, pspecs[i]->name,
                                          parse_object_property(G_OBJECT(element), pspecs[i]));
    }

    g_value_take_boxed(result, dictionary);
}
