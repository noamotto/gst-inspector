/*
 * Implementation of GObject type reader
 */
#include "type_reader/type_reader_priv.h"

static GHashTable *object_map = NULL;

/**
 *  @addtogroup readers
 *  @{
 */

/**
 *  @brief Registers a new object content reader
 *
 *  Registers an object content reader, a function for parsing data about a 
 *  specific type of GObject.
 * 
 *  @param object_type Type of GObject to register the content reader for
 *  @param read_func Content reader function
 * 
 *  @returns TRUE if registering succeeded. Otherwise FALSE is returned.
 */
gboolean gst_object_content_reader_register(GType object_type,
                                            GstObjectContentReaderFunc read_func)
{
    if (G_UNLIKELY(!object_map))
    {
        object_map = g_hash_table_new(g_direct_hash, g_direct_equal);
    }

    g_return_val_if_fail(g_type_is_a(object_type, G_TYPE_OBJECT), FALSE);

    if (!g_hash_table_contains(object_map, GINT_TO_POINTER(object_type)))
    {
        g_hash_table_insert(object_map, GINT_TO_POINTER(object_type), (gpointer)read_func);
    }
    else
    {
        g_warning(
            "Trying to register a content parser for existing object type %s. Not registering.",
            g_type_name(object_type));
        return FALSE;
    }

    return TRUE;
}

/** @}*/

void gst_object_reader_parse_content(GObject *object,
                                     GstStructure *dictionary)
{
    GstObjectContentReaderFunc read_func = NULL;
    g_return_if_fail(G_IS_OBJECT(object) || !object);

    if (G_UNLIKELY(!object_map))
    {
        object_map = g_hash_table_new(g_direct_hash, g_direct_equal);
    }

    if (object)
    {
        read_func = (GstObjectContentReaderFunc)g_hash_table_lookup(object_map,
                                                                    GINT_TO_POINTER(G_OBJECT_TYPE(object)));

        if (read_func)
        {
            read_func(object, dictionary);
        }
    }
}

/**
 *  @addtogroup type-readers
 *  @subsection object-reader Object type reader
 *  The object type reader (for G_TYPE_OBJECT and subtypes) parses the following 
 *  additional fields:
 *  - <b>Type</b> - Property's object value type
 * 
 *  For additional parsed fields see the available @ref object-readers 
 *      "object content readers"
 */
void gst_object_type_reader_fill_type(
    GParamSpec *pspec,
    GValue *value,
    GstStructure *dictionary)
{
    GObject *object = NULL;

    g_return_if_fail(G_IS_PARAM_SPEC_OBJECT(pspec));
    g_return_if_fail(G_VALUE_HOLDS_OBJECT(value));

    object = g_value_get_object(value);

    gst_dictionary_set_string(dictionary, KEY_TYPE,
                              g_strdup_printf("Object of type \"%s\"",
                                              g_type_name(pspec->value_type)));

    gst_object_reader_parse_content(object, dictionary);
}
