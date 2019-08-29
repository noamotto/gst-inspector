/**
 *  @file gstelementpresetsinspector.c
 *  @brief Element preset inspector implementation
 */
#include "gstinspectors.h"
#include "gstinspector_priv.h"

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
 *  If the element implements GstPreset, the inspected data is an array of preset
 *  names.
 *  @endparblock
 */
void gst_inspector_inspect_element_presets(GstElement *element, GValue *result)
{
    g_return_if_fail(GST_IS_ELEMENT(element));

    if (GST_IS_PRESET(element))
    {
        gchar **presets = gst_preset_get_preset_names(GST_PRESET(element));

        if (presets)
        {
            if(*presets)
            {
                g_value_init(result, GST_TYPE_LIST);

                for (gchar **preset = presets; *preset; preset++)
                {
                    gst_array_append_string(result, g_strdup(*preset));
                }
            }
            g_strfreev(presets);
        }
    }
}
