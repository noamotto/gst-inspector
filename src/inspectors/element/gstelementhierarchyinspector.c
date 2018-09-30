#include "gstinspectors.h"
#include "gstinspector_priv.h"

GstStructure *gst_inspector_inspect_element_hierarchy(GstElement *element)
{
    GstStructure *dictionary = NULL;

    g_return_val_if_fail(GST_IS_ELEMENT(element), NULL);

    dictionary = gst_structure_new_empty("hierarchy");
    gst_dictionary_set_array(dictionary, "Hierarchy", parse_type_hierarchy(G_OBJECT_TYPE(element)));

    return dictionary;
}
