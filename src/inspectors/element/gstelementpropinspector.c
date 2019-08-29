/**
 *  @file gstelementpropinspector.c
 *  @brief Element properties inspector implementation
 */
#include "gstinspectors.h"
#include "gstinspector_priv.h"
#include "type_reader/type_reader.h"

static gint cmp_pspecs(GParamSpec **p1, GParamSpec **p2)
{
    return g_strcmp0(g_param_spec_get_name(*p1), g_param_spec_get_name(*p2));
}

/**
 *  @brief Inspects element's presets
 * 
 *  Inspects the preset names of a single element
 * 
 *  @param element Element to inspect
 *  @param result 
 *  @parblock
 *  The inspected data
 * 
 *  The inspected data is an array of the parse element properties. Each property 
 *      consists of these fields:
 *  - <b>Name</b> - The property name and blurb, in the format 
 *      <tt>[name]: [blurb]</tt>
 *  - <b>Flags</b> - Array of strings describing the property's flags
 *  - Additional fields depending on the property type. Refer to the respective type 
 *      reader for those fields
 *  @endparblock
 */
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
    g_free(pspecs);
}
