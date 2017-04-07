#include "type_reader/type_reader_priv.h"

static GHashTable *boxed_map = NULL;

gboolean gst_boxed_content_reader_register(GType boxed_type,
                                           GstBoxedContentReaderFunc read_func)
{
    if (G_UNLIKELY(!boxed_map))
    {
        g_hash_table_new(g_direct_hash, g_direct_equal);
    }

    g_return_val_if_fail(g_type_is_a(boxed_type, G_TYPE_BOXED), FALSE);

    if (!g_hash_table_contains(boxed_map, GINT_TO_POINTER(boxed_type)))
    {
        g_hash_table_insert(boxed_map, GINT_TO_POINTER(boxed_type), (gpointer)read_func);
    }
    else
    {
        g_warning(
            "Trying to register a content parser for existing boxed type %s. Not registering.",
            g_type_name(boxed_type));
        return FALSE;
    }

    return TRUE;
}

void gst_boxed_reader_parse_content(GValue* const boxed,
                                    GstStructure *const dictionary)
{
    GstBoxedContentReaderFunc read_func = NULL;
    g_return_if_fail(G_VALUE_HOLDS_BOXED(boxed));

    if (G_UNLIKELY(!boxed_map))
    {
        g_hash_table_new(g_direct_hash, g_direct_equal);
    }

    read_func = (GstBoxedContentReaderFunc)g_hash_table_lookup(boxed_map,
                                                               GINT_TO_POINTER(boxed));

    if (read_func)
    {
        read_func(boxed, dictionary);
    }
}

void gst_boxed_type_reader_fill_type(
    GParamSpec *const pspec,
    GValue *const value,
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

    gst_boxed_reader_parse_content(value, dictionary);
}
