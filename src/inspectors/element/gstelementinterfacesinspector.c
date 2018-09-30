#include "gstinspectors.h"
#include "gstinspector_priv.h"

GstStructure *gst_inspector_inspect_element_interfaces(GstElement *element)
{
    GstStructure *dictionary = NULL;
    GArray *ifaces;

    g_return_val_if_fail(GST_IS_ELEMENT(element), NULL);

    ifaces = parse_type_interfaces(G_OBJECT_TYPE(element));

    if (ifaces)
    {
        dictionary = gst_structure_new_empty("interfaces");
        gst_dictionary_set_array(dictionary, "Implemented Interfaces", ifaces);
    }
    return dictionary;
}
