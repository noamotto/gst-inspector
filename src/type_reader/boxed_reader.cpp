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

void BoxedTypeReader::print_caps(
    const GstCaps *caps,
    param_keys &keys) const
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
            gst_structure_foreach(structure, GstStructureForeachFunc(print_field), caps_string);
        }
    }

    keys[KEY_VALUE] = g_string_free(caps_string, FALSE);
}

void BoxedTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    param_keys &keys) const
{
    keys[KEY_TYPE] = g_strdup_printf("%-23.23s Boxed pointer of type \"%s\"", "",
                                     g_type_name(pspec->value_type));

    if (pspec->value_type == GST_TYPE_STRUCTURE)
    {
        GString *struct_string = g_string_new(NULL);
        const GstStructure *structure = gst_value_get_structure(value);
        if (structure)
        {
            gst_structure_foreach(structure, GstStructureForeachFunc(print_field),
                                  struct_string);
        }
        keys[KEY_VALUE] = g_string_free(struct_string, FALSE);
    }
    else if (pspec->value_type == GST_TYPE_CAPS)
    {
        const GstCaps *caps = gst_value_get_caps(value);

        if (!caps)
        {
            keys[KEY_VALUE] = g_strdup("Caps (NULL)");
        }
        else
        {
            print_caps(caps, keys);
        }
    }
}