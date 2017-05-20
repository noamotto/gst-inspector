#include "type_reader/type_reader_priv.h"

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

void gst_structure_content_reader_parse(GValue *boxed,
                                        GstStructure *dictionary)
{
    const GstStructure *structure;
    GArray *fields_array;
    GValue key_value = G_VALUE_INIT;

    g_return_if_fail(GST_VALUE_HOLDS_STRUCTURE(boxed));

    fields_array = g_array_new(FALSE, FALSE, sizeof(GValue));
    g_array_set_clear_func(fields_array, (GDestroyNotify)g_value_unset);

    structure = gst_value_get_structure(boxed);

    if (structure)
    {
        gst_structure_foreach(structure,
                              (GstStructureForeachFunc)print_field,
                              fields_array);
    }

    g_value_init(&key_value, G_TYPE_ARRAY);
    g_value_take_boxed(&key_value, fields_array);
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}
