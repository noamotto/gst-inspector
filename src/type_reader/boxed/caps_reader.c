#include "type_reader/type_reader_priv.h"

static gboolean
print_field(
    GQuark field_id,
    const GValue *value,
    GString *struct_string)
{
    gchar *str = gst_value_serialize(value);

    gchar *field = g_strdup_printf("%15s: %s\n",
                                   g_quark_to_string(field_id),
                                   str);

    struct_string = g_string_append(struct_string, field);

    g_free(str);
    g_free(field);
    return TRUE;
}

static void parse_caps_fields(const GstCaps *caps,
                              GString *caps_string)
{
    for (guint i = 0; i < gst_caps_get_size(caps); i++)
    {
        GstStructure *structure = gst_caps_get_structure(caps, i);
        GstCapsFeatures *features = gst_caps_get_features(caps, i);

        if (features && (gst_caps_features_is_any(features) ||
                         !gst_caps_features_is_equal(features,
                                                     GST_CAPS_FEATURES_MEMORY_SYSTEM_MEMORY)))
        {
            gchar *features_string = gst_caps_features_to_string(features);

            g_string_append(caps_string, gst_structure_get_name(structure));
            g_string_append_c(caps_string, '(');
            g_string_append(caps_string, features_string);
            g_string_append_c(caps_string, ')');
            g_free(features_string);
        }
        else
        {
            g_string_append(caps_string, gst_structure_get_name(structure));
        }
        gst_structure_foreach(structure,
                              (GstStructureForeachFunc)print_field,
                              caps_string);
    }
}

static gchar *parse_caps(
    const GstCaps *caps)
{
    GString *caps_string = g_string_new(NULL);
    if (gst_caps_is_any(caps))
    {
        g_string_append(caps_string, "ANY");
    }
    else if (gst_caps_is_empty(caps))
    {
        g_string_append(caps_string, "EMPTY");
    }
    else
    {
        parse_caps_fields(caps, caps_string);
    }

    return g_string_free(caps_string, FALSE);
}

void gst_caps_content_reader_parse(
    GValue *const boxed,
    GstStructure *const dictionary)
{
    const GstCaps *caps;
    GValue key_value = G_VALUE_INIT;

    g_return_if_fail(GST_VALUE_HOLDS_CAPS(boxed));

    caps = gst_value_get_caps(boxed);
    if (!caps)
    {
        g_value_init(&key_value, G_TYPE_STRING);
        g_value_take_string(&key_value, g_strdup("Caps (NULL)"));
        gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
    }
    else
    {
        g_value_init(&key_value, G_TYPE_STRING);
        g_value_take_string(&key_value, parse_caps(caps));
        gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
    }
}
