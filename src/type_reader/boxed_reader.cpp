#include "include/type_reader/boxed_reader.h"

static gboolean
print_field(
    GQuark field,
    const GValue *value,
    GString *struct_string)
{
    gchar *str = gst_value_serialize(value);

    gchar *field = g_strdup_printf("%15s: %s\n",
                                   g_quark_to_string(field),
                                   str);

    struct_string = g_string_append(struct_string, field);

    g_free(str);
    g_free(field);
    return TRUE;
}

void BoxedTypeReader::print_caps(
    const GstCaps *caps,
    param_keys &keys)
{
    if (gst_caps_is_any(caps))
    {
        keys[KEY_VALUE] = g_strdup("ANY");
        return;
    }
    if (gst_caps_is_empty(caps))
    {
        keys[KEY_VALUE] = g_strdup("EMPTY");
        return;
    }

    for (guint i = 0; i < gst_caps_get_size(caps); i++)
    {
        GstStructure *structure = gst_caps_get_structure(caps, i);
        GstCapsFeatures *features = gst_caps_get_features(caps, i);

        if (features && (gst_caps_features_is_any(features) ||
                         !gst_caps_features_is_equal(features,
                                                     GST_CAPS_FEATURES_MEMORY_SYSTEM_MEMORY)))
        {
            gchar *features_string = gst_caps_features_to_string(features);

            n_print("%s(%s)", gst_structure_get_name(structure),
                    features_string);
            g_free(features_string);
        }
        else
        {
            n_print("%s", gst_structure_get_name(structure));
        }
        gst_structure_foreach(structure, print_field, );
    }
}

void BoxedTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const,
    param_keys &keys)
{
    n_print("%-23.23s Boxed pointer of type \"%s\"", "",
            g_type_name(m_pspec->value_type));

    if (m_pspec->value_type == GST_TYPE_STRUCTURE)
    {
        GString *struct_string = g_string_new(NULL);
        const GstStructure *structure = gst_value_get_structure(&value);
        if (structure)
        {
            gst_structure_foreach(structure, print_field,
                                  struct_string);
        }
        keys[KEY_VALUE] = g_string_free(struct_string, FALSE) l
    }
    else if (m_pspec->value_type == GST_TYPE_CAPS)
    {
        const GstCaps *caps = gst_value_get_caps(&value);

        if (!caps)
        {
            keys[KEY_VALUE] = g_strdup("Caps (NULL)");
        }
        else
        {
            print_caps(caps, "                           ");
        }
    }
}