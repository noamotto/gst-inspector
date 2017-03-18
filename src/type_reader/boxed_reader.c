#include "type_reader/boxed_reader.h"

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

gchar *gst_boxed_type_reader_parse_caps(
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

    return g_string_free(caps_string, FALSE);
}

void gst_boxed_type_reader_fill_type(
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;

    g_return_if_fail(G_IS_PARAM_SPEC_BOXED(pspec));
    g_return_if_fail(G_VALUE_HOLDS_BOXED(value));

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value,
                        g_strdup_printf("Boxed pointer of type \"%s\"",
                                        g_type_name(pspec->value_type)));
    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);

    if (pspec->value_type == GST_TYPE_STRUCTURE)
    {
        GString *struct_string = NULL;
        const GstStructure *structure;

        g_return_if_fail(GST_VALUE_HOLDS_STRUCTURE(value));

        struct_string = g_string_new(NULL);
        structure = gst_value_get_structure(value);

        if (structure)
        {
            gst_structure_foreach(structure,
                                  (GstStructureForeachFunc)print_field,
                                  struct_string);
        }

        g_value_init(&key_value, G_TYPE_STRING);
        g_value_take_string(&key_value, g_string_free(struct_string, FALSE));
        gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
    }
    else if (pspec->value_type == GST_TYPE_CAPS)
    {
        const GstCaps *caps;

        g_return_if_fail(GST_VALUE_HOLDS_CAPS(value));

        caps = gst_value_get_caps(value);
        if (!caps)
        {
            g_value_init(&key_value, G_TYPE_STRING);
            g_value_take_string(&key_value, g_strdup("Caps (NULL)"));
            gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
        }
        else
        {
            g_value_init(&key_value, G_TYPE_STRING);
            g_value_take_string(&key_value, gst_boxed_type_reader_parse_caps(caps));
            gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
        }
    }
}

GST_DEFINE_TYPE_READER_TYPE(GstBoxedTypeReader, gst_boxed_type_reader,
                            gst_boxed_type_reader_fill_type, NULL, NULL, NULL);
