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
        GArray *clocking_data = g_array_new(FALSE, FALSE, sizeof(GValue));
        g_array_set_clear_func(clocking_data, (GDestroyNotify)g_value_unset);
        g_value_init(result, G_TYPE_ARRAY);

        if (requires_clock)
        {
            g_array_add_static_string(clocking_data, "element requires a clock\n");
        }

        if (provides_clock)
        {
            GstClock *clock;

            clock = gst_element_get_clock(element);
            if (clock)
            {
                g_array_add_string(clocking_data,
                                   g_strdup_printf("element provides a clock: %s\n",
                                                   GST_OBJECT_NAME(clock)));
                gst_object_unref(clock);
            }
            else
            {
                g_array_add_static_string(clocking_data,
                                          "element is supposed to provide a clock but returned NULL");
            }
        }
    }
}
