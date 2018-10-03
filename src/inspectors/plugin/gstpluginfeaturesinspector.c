#include "gstinspectors.h"
#include "gstinspector_priv.h"

typedef struct _InspectedPluginFeatures
{
    GstPlugin *plugin;
    GArray *elements_array;
    GArray *typefinds_array;
    GArray *tracers_array;
    GArray *devproviders_array;
    GArray *others_array;
    guint num_features;
} InspectedPluginFeatures;

static void parse_typefind(GstPluginFeature *feature, InspectedPluginFeatures *features)
{
    GstTypeFindFactory *factory;
    const gchar *const *extensions;
    GString *typefind_str = g_string_new("");

    factory = GST_TYPE_FIND_FACTORY(feature);
    extensions = gst_type_find_factory_get_extensions(factory);

    g_string_printf(typefind_str, "%s: %s: ", gst_plugin_get_name(features->plugin),
                    gst_plugin_feature_get_name(feature));
    if (extensions)
    {
        guint i = 0;

        while (extensions[i])
        {
            g_string_append_printf(typefind_str, "%s%s", i > 0 ? ", " : "", extensions[i]);
            i++;
        }
        g_print("\n");
    }
    else
    {
        g_string_append(typefind_str, "no extensions");
    }

    if (G_UNLIKELY(!features->typefinds_array))
    {
        G_VALUE_ARRAY_NEW(features->typefinds_array);
    }

    g_array_add_string(features->typefinds_array, g_string_free(typefind_str, FALSE));
}

static void parse_feature(GstPluginFeature *feature, InspectedPluginFeatures *features)
{
    if (GST_IS_ELEMENT_FACTORY(feature))
    {
        GstElementFactory *factory = GST_ELEMENT_FACTORY(feature);

        if (G_UNLIKELY(!features->elements_array))
        {
            G_VALUE_ARRAY_NEW(features->elements_array);
        }
        g_array_add_string(features->elements_array,
                           g_strdup_printf("%s: %s", GST_OBJECT_NAME(factory),
                                           gst_element_factory_get_metadata(factory,
                                                                            GST_ELEMENT_METADATA_LONGNAME)));
    }
    else if (GST_IS_TYPE_FIND_FACTORY(feature))
    {
        parse_typefind(feature, features);
    }
    else if (GST_IS_DEVICE_PROVIDER_FACTORY(feature))
    {
        GstDeviceProviderFactory *factory = GST_DEVICE_PROVIDER_FACTORY(feature);

        if (G_UNLIKELY(!features->devproviders_array))
        {
            G_VALUE_ARRAY_NEW(features->devproviders_array);
        }

        g_array_add_string(features->devproviders_array,
                           g_strdup_printf("%s: %s", GST_OBJECT_NAME(factory),
                                           gst_device_provider_factory_get_metadata(factory,
                                                                                    GST_ELEMENT_METADATA_LONGNAME)));
    }
    else if (GST_IS_TRACER_FACTORY(feature))
    {
        if (G_UNLIKELY(!features->tracers_array))
        {
            G_VALUE_ARRAY_NEW(features->tracers_array);
        }

        g_array_add_string(features->tracers_array,
                           g_strdup_printf("%s (%s)", gst_object_get_name(GST_OBJECT(feature)),
                                           g_type_name(G_OBJECT_TYPE(feature))));
    }
    else if (feature)
    {
        if (G_UNLIKELY(!features->others_array))
        {
            G_VALUE_ARRAY_NEW(features->others_array);
        }

        g_array_add_string(features->others_array,
                           g_strdup_printf("%s (%s)", gst_object_get_name(GST_OBJECT(feature)),
                                           g_type_name(G_OBJECT_TYPE(feature))));
    }

    features->num_features++;
}

void gst_inspector_inspect_plugin_features(GstPlugin *plugin, GValue *result)
{
    GstStructure *dictionary;
    GList *features;
    InspectedPluginFeatures *inspected_features =
        (InspectedPluginFeatures *)g_new0(InspectedPluginFeatures, 1);

    features = gst_registry_get_feature_list_by_plugin(gst_registry_get(),
                                                       gst_plugin_get_name(plugin));

    dictionary = gst_structure_new_empty("pluginfeatures");
    g_value_init(result, GST_TYPE_STRUCTURE);

    g_list_foreach(features, (GFunc)parse_feature, inspected_features);

    gst_plugin_feature_list_free(features);

    gst_dictionary_set_string(dictionary, "Total features",
                              g_strdup_printf("%u", inspected_features->num_features));

    if (inspected_features->elements_array)
    {
        gst_dictionary_set_array(dictionary, "Elements", inspected_features->elements_array);
    }
    if (inspected_features->typefinds_array)
    {
        gst_dictionary_set_array(dictionary, "Typefinders", inspected_features->typefinds_array);
    }
    if (inspected_features->devproviders_array)
    {
        gst_dictionary_set_array(dictionary, "Device providers",
                                 inspected_features->devproviders_array);
    }
    if (inspected_features->tracers_array)
    {
        gst_dictionary_set_array(dictionary, "Tracers", inspected_features->tracers_array);
    }
    if (inspected_features->others_array)
    {
        gst_dictionary_set_array(dictionary, "Other objects", inspected_features->others_array);
    }

    g_value_take_boxed(result, dictionary);
}
