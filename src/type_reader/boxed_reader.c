/*
 * Implementation of GObject type reader
 */
#include "type_reader/type_reader_priv.h"

static GHashTable *boxed_map = NULL;

/**
 *  @addtogroup readers
 *  @{
 */

/**
 *  @brief Registers a new boxed content reader
 *
 *  Registers a boxed content reader, a function for parsing data about a 
 *  specific type of GBoxed.
 * 
 *  @param boxed_type Type of GBoxed to register the content reader for
 *  @param read_func Content reader function
 * 
 *  @returns TRUE if registering succeeded. Otherwise FALSE is returned.
 */
gboolean gst_boxed_content_reader_register(GType boxed_type,
                                           GstBoxedContentReaderFunc read_func)
{
    if (G_UNLIKELY(!boxed_map))
    {
        boxed_map = g_hash_table_new(g_direct_hash, g_direct_equal);
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

/** @}*/

void gst_boxed_reader_parse_content(GValue *boxed,
                                    GstStructure *dictionary)
{
    GstBoxedContentReaderFunc read_func = NULL;
    g_return_if_fail(G_VALUE_HOLDS_BOXED(boxed));

    if (G_UNLIKELY(!boxed_map))
    {
        g_hash_table_new(g_direct_hash, g_direct_equal);
    }

    read_func = (GstBoxedContentReaderFunc)
        g_hash_table_lookup(boxed_map, GINT_TO_POINTER(boxed->g_type));

    if (read_func)
    {
        read_func(boxed, dictionary);
    }
}

/**
 *  @addtogroup type-readers
 *  @subsection boxed-reader Boxed type reader
 *  The boxed type reader (for G_TYPE_BOXED and subtypes) parses the following 
 *  additional fields:
 *  - <b>Type</b> - Property's boxed value type
 * 
 *  For additional parsed fields see the available boxed content readers
 */
void gst_boxed_type_reader_fill_type(GParamSpec *pspec,
                                     GValue *value,
                                     GstStructure *dictionary)
{
    g_return_if_fail(G_IS_PARAM_SPEC_BOXED(pspec));
    g_return_if_fail(G_VALUE_HOLDS_BOXED(value));

    gst_dictionary_set_string(dictionary, KEY_TYPE,
                              g_strdup_printf("Boxed pointer of type \"%s\"",
                                              g_type_name(pspec->value_type)));

    gst_boxed_reader_parse_content(value, dictionary);
}
