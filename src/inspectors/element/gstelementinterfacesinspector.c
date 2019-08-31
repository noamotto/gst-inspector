/*
 *  Element interfaces inspector implementation
 */
#include "gstinspectors.h"
#include "gstinspector_priv.h"

/** 
 *  @addtogroup element-inspectors 
 *  @{ 
 */

/**
 *  @brief Inspects element implemented interfaces
 * 
 *  Inspects the implemented interfaces of a single element
 * 
 *  @param element Element to inspect
 *  @param result 
 *  @parblock
 *  The inspected data
 * 
 *  The inspected data is an array of string, where each string is the name of one
 *  of the element's implemented interfaces.
 *  @endparblock
 */
void gst_inspector_inspect_element_interfaces(GstElement *element, GValue *result)
{
    g_return_if_fail(GST_IS_ELEMENT(element));

    parse_type_interfaces(G_OBJECT_TYPE(element), result);
}

/** @}*/
