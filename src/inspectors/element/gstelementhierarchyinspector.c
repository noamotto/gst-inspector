/**
 *  @file gstelementhierarchyinspector.c
 *  @brief Element hierarchy inspector implementation
 */
#include "gstinspectors.h"
#include "gstinspector_priv.h"

/**
 *  @brief Inspects element type hierarchy
 * 
 *  Inspects the type hierarchy of a single element
 * 
 *  @param element Element to inspect
 *  @param result 
 *  @parblock
 *  The inspected data
 * 
 *  The inspected data is an array where each entry is a type name in the element's
 *  type hierarchy. The array is ordered such that the first entry is GObject and the
 *  last entry is the element's type name.
 *  @endparblock
 */
void gst_inspector_inspect_element_hierarchy(GstElement *element, GValue *result)
{
    g_return_if_fail(GST_IS_ELEMENT(element));

    parse_type_hierarchy(G_OBJECT_TYPE(element), result);
}
