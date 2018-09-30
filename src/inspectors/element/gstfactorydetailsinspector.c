#include "gstinspectors.h"
#include "gstinspector_priv.h"

GstStructure *gst_inspector_inspect_factory_details(GstElement *element)
{
    GstStructure *dictionary;
    GstElementFactory *factory;
    gchar **keys, **k;
    guint rank;

    g_return_val_if_fail(GST_IS_ELEMENT(element), NULL);

    factory = gst_element_get_factory(element);
    dictionary = gst_structure_new_empty("factorydetails");

    rank = gst_plugin_feature_get_rank(GST_PLUGIN_FEATURE(factory));

    gst_dictionary_set_string(dictionary, "Rank",
                              g_strdup_printf("%s (%d)\n", get_rank_name((gint)rank), rank));

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

    return dictionary;
}
