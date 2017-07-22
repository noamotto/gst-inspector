#include "type_reader/type_reader_priv.h"

#define KEY_FEATURES ("Features")

static gboolean
print_field(
    GQuark field_id,
    const GValue *value,
    GArray *array)
{
    GValue key_value = G_VALUE_INIT;
    gchar *str = gst_value_serialize(value);
    gchar *field = g_strdup_printf("%s: %s",
                                   g_quark_to_string(field_id),
                                   str);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, field);
    g_array_append_val(array, key_value);

    g_free(str);
    return TRUE;
}

static GArray *parse_caps(const GstCaps *caps)
{
    GArray *caps_array = g_array_new(FALSE, FALSE, sizeof(GValue));
    GValue caps_value = G_VALUE_INIT;

    g_array_set_clear_func(caps_array, (GDestroyNotify)g_value_unset);

    for (guint i = 0; i < gst_caps_get_size(caps); i++)
    {
        GstStructure *caps_dict = gst_structure_new_empty("caps_dict");
        GArray *fields_array = g_array_new(FALSE, TRUE, sizeof(GValue));
        GstStructure *structure = gst_caps_get_structure(caps, i);
        GstCapsFeatures *features = gst_caps_get_features(caps, i);

        gst_dictionary_set_static_string(caps_dict, KEY_TYPE, gst_structure_get_name(structure));

        if (features && (gst_caps_features_is_any(features) ||
                         !gst_caps_features_is_equal(features,
                                                     GST_CAPS_FEATURES_MEMORY_SYSTEM_MEMORY)))
        {
            gst_dictionary_set_string(caps_dict, KEY_FEATURES,
                                      gst_caps_features_to_string(features));
        }
        gst_structure_foreach(structure,
                              (GstStructureForeachFunc)print_field,
                              fields_array);
        g_array_set_clear_func(fields_array, (GDestroyNotify)g_value_unset);

        gst_dictionary_set_array(caps_dict, KEY_VALUE, fields_array);

        g_value_init(&caps_value, GST_TYPE_STRUCTURE);
        g_value_take_boxed(&caps_value, caps_dict);
        g_array_append_val(caps_array, caps_value);
    }

    return caps_array;
}

void gst_caps_content_reader_parse(GValue *boxed,
                                   GstStructure *dictionary)
{
    const GstCaps *caps;

    g_return_if_fail(GST_VALUE_HOLDS_CAPS(boxed));

    caps = gst_value_get_caps(boxed);
    if (!caps)
    {
        gst_dictionary_set_static_string(dictionary, KEY_VALUE, "Caps (NULL)");
    }
    else if (gst_caps_is_any(caps))
    {
        gst_dictionary_set_static_string(dictionary, KEY_VALUE, "ANY");
    }
    else if (gst_caps_is_empty(caps))
    {
        gst_dictionary_set_static_string(dictionary, KEY_VALUE, "EMPTY");
    }
    else
    {
        gst_dictionary_set_array(dictionary, KEY_VALUE, parse_caps(caps));
    }
}
