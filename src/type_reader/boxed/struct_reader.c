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

void gst_structure_content_reader_parse(
    GValue *const boxed,
    GstStructure *const dictionary)
{
    GString *struct_string = NULL;
    const GstStructure *structure;
    GValue key_value = G_VALUE_INIT;

    g_return_if_fail(GST_VALUE_HOLDS_STRUCTURE(boxed));

    struct_string = g_string_new(NULL);
    structure = gst_value_get_structure(boxed);

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
