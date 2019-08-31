/*
 *  Element clocking capabilities inspector implementation
 */
#include "gstinspectors.h"
#include "gstinspector_priv.h"

/** 
 *  @addtogroup element-inspectors 
 *  @{ 
 */

/**
 *  @brief Inspects element clocking capabilities
 * 
 *  Inspects the clocking capabilities of a single element
 * 
 *  @param element Element to inspect
 *  @param result 
 *  @parblock
 *  The inspected data
 * 
 *  The inspected data depends on the element's clocking capabilities:
 *  - If the element does not require a clock nor provides one, a string 
 *      is returned informing about it.
 *  - Otherwise, an array is returned:
 *  -- If the element requires a clock then a string informing so is present
 *      in the result.
 *  -- If the element provides a clock then a string informing so is present
 *      in the result.
 *  -- If the element says it provides a clock but returns NULL on gst_element_get_clock() 
 *      then a string informing so is present in the result.
 *  @endparblock
 */
void gst_inspector_inspect_element_clocking(GstElement *element, GValue *result)
{
    gboolean requires_clock, provides_clock;

    g_return_if_fail(GST_IS_ELEMENT(element));

    requires_clock = GST_OBJECT_FLAG_IS_SET(element, GST_ELEMENT_FLAG_REQUIRE_CLOCK);
    provides_clock = GST_OBJECT_FLAG_IS_SET(element, GST_ELEMENT_FLAG_PROVIDE_CLOCK);

    if (!requires_clock && !provides_clock)
    {
        g_value_init(result, G_TYPE_STRING);
        g_value_set_static_string(result, "Element has no clocking capabilities.");
    }
    else
    {
        g_value_init(result, GST_TYPE_LIST);

        if (requires_clock)
        {
            gst_array_append_static_string(result, "element requires a clock");
        }

        if (provides_clock)
        {
            GstClock *clock;

            clock = gst_element_get_clock(element);
            if (clock)
            {
                gst_array_append_string(result,
                                        g_strdup_printf("element provides a clock: %s",
                                                        GST_OBJECT_NAME(clock)));
                gst_object_unref(clock);
            }
            else
            {
                gst_array_append_static_string(result,
                                               "element is supposed to provide a clock but returned NULL");
            }
        }
    }
}

/** @}*/
