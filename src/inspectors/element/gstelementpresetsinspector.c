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
            GArray *presets_array = g_array_new(FALSE, FALSE, sizeof(GValue));
            g_array_set_clear_func(presets_array, (GDestroyNotify)g_value_unset);
            g_value_init(result, G_TYPE_ARRAY);

            for (gchar **preset = presets; *preset; preset++)
            {
                g_array_add_string(presets_array, g_strdup(*preset));
            }

            g_value_take_boxed(result, presets_array);
            g_strfreev(presets);
        }
    }
}
