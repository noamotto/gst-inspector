#include "gstinspectors.h"
#include "gstinspector_priv.h"

void gst_inspector_inspect_element_presets(GstElement *element, GValue *result)
{
    g_return_if_fail(GST_IS_ELEMENT(element));

    if (GST_IS_PRESET(element))
    {
        gchar **presets = gst_preset_get_preset_names(GST_PRESET(element));

        if (presets && *presets)
        {
            g_value_init(result, GST_TYPE_ARRAY);

            for (gchar **preset = presets; *preset; preset++)
            {
                gst_array_append_string(result, g_strdup(*preset));
            }

            g_strfreev(presets);
        }
    }
}
