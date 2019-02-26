#include "gstinspectors.h"
#include "gstinspector_priv.h"

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
