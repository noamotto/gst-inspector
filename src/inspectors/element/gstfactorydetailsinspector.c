/*
 *  Element factory details inspector implementation
 */
#include "gstinspectors.h"
#include "gstinspector_priv.h"

/** 
 *  @addtogroup element-inspectors 
 *  @{ 
 */

/**
 *  @brief Inspects an element's factory details
 * 
 *  Inspects the factory details of a single element
 * 
 *  @param element Element to inspect
 *  @param result 
 *  @parblock
 *  The inspected data
 * 
 *  The inspected data consists of the following fields:
 *  - <b>Rank</b> - The element's rank, for autoplugging to choose the most 
 *      appropriate element
 *  - Additional metadata field, where the metadata key serves as the field key,
 *      and the metadata value serves as the field's value. For example:
 *      - <b>Long-name</b> - The element's long name
 *      - <b>Description</b> - Short description of the element
 *  @endparblock
 */
void gst_inspector_inspect_factory_details(GstElement *element, GValue *result)
{
    GstStructure *dictionary;
    GstElementFactory *factory;
    gchar **keys, **k;
    guint rank;

    g_return_if_fail(GST_IS_ELEMENT(element));

    factory = gst_element_get_factory(element);
    dictionary = gst_structure_new_empty("factorydetails");
    g_value_init(result, GST_TYPE_STRUCTURE);

    rank = gst_plugin_feature_get_rank(GST_PLUGIN_FEATURE(factory));

    gst_dictionary_set_string(dictionary, "Rank", get_rank_name((gint)rank));

    keys = gst_element_factory_get_metadata_keys(factory);
    if (keys != NULL)
    {
        for (k = keys; *k != NULL; ++k)
        {
            const gchar *val;
            gchar *key = *k;

            val = gst_element_factory_get_metadata(factory, key);
            key[0] = g_ascii_toupper(key[0]);
            gst_dictionary_set_static_string(dictionary, key, val);
        }
        g_strfreev(keys);
    }

    g_value_take_boxed(result, dictionary);
}

/** @}*/
