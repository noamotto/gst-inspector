#include "gstinspector.h"
#include "gsttestplugin.h"
#include "testutils.h"

#define TYPEFIND_NAME ("testtypefind")

static GArray *parse_extensions(GstTypeFindFactory *factory)
{
    const gchar *const *extensions = gst_type_find_factory_get_extensions(factory);
    if (extensions && *extensions)
    {
        GArray *extensions_arr = g_array_new(FALSE, FALSE, sizeof(GValue));
        g_array_set_clear_func(extensions_arr, (GDestroyNotify)g_value_unset);
        guint i = 0;

        while (extensions[i])
        {
            g_array_add_static_string(extensions_arr, extensions[i]);
            i++;
        }
        return extensions_arr;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    GstStructure *data;
    GstStructure *factory_data;
    GArray *caps_arr, *extensions_arr;
    GstCaps *caps;
    GstPluginFeature *feature;
    guint rank;
    gchar *rank_string;

    gst_init(&argc, &argv);
    GST_PLUGIN_STATIC_REGISTER(testplugin);

    data = gst_inspector_inspect_by_name(TYPEFIND_NAME);
    feature = gst_registry_find_feature(gst_registry_get(), TYPEFIND_NAME,
                                        GST_TYPE_TYPE_FIND_FACTORY);

    g_assert_cmpstr(TYPEFIND_NAME, ==, gst_structure_get_name(data));

    g_assert_true(gst_structure_has_field_typed(data, "Factory Details", GST_TYPE_STRUCTURE));
    factory_data = gst_dictionary_get_sub_dictionary(data, "Factory Details");

    g_assert_true(gst_structure_has_field_typed(factory_data, "Name", G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(factory_data, "Name"), ==, TYPEFIND_NAME);

    g_assert_true(gst_structure_has_field_typed(factory_data, "Rank", G_TYPE_STRING));
    rank = gst_plugin_feature_get_rank(feature);
    rank_string = get_rank_name((gint)rank);
    g_assert_cmpstr(gst_dictionary_get_string(factory_data, "Rank"), ==, rank_string);
    g_free(rank_string);

    caps = gst_type_find_factory_get_caps(GST_TYPE_FIND_FACTORY(feature));

    if (caps)
    {
        caps_arr = parse_caps(caps);
        g_assert_true(gst_structure_has_field_typed(factory_data, "Caps", G_TYPE_ARRAY));
        compare_arrays(caps_arr, gst_dictionary_get_array(data, "Caps"));
        g_array_free(caps_arr, TRUE);
    }
    else
    {
        g_assert_false(gst_structure_has_field(factory_data, "Caps"));
    }

    extensions_arr = parse_extensions(GST_TYPE_FIND_FACTORY(feature));
    if (extensions_arr)
    {
        g_assert_true(gst_structure_has_field_typed(factory_data, "Extensions", G_TYPE_ARRAY));
        compare_arrays(extensions_arr, gst_dictionary_get_array(data, "Extension"));
        g_array_free(extensions_arr, TRUE);
    }
    else
    {
        g_assert_false(gst_structure_has_field(factory_data, "Extensions"));
    }

    g_assert_true(gst_structure_has_field_typed(data, "Plugin Details", GST_TYPE_STRUCTURE));
    check_plugin_detais(gst_plugin_feature_get_plugin(feature),
                        gst_dictionary_get_sub_dictionary(data, "Plugin Details"));

    gst_object_unref(feature);
    gst_structure_free(data);

    return 0;
}
