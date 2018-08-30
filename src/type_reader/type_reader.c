#include "type_reader/type_reader_priv.h"

#define KNOWN_PARAM_FLAGS                                       \
    (G_PARAM_CONSTRUCT | G_PARAM_CONSTRUCT_ONLY |               \
     G_PARAM_LAX_VALIDATION | G_PARAM_STATIC_STRINGS |          \
     G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_DEPRECATED | \
     GST_PARAM_CONTROLLABLE | GST_PARAM_MUTABLE_PLAYING |       \
     GST_PARAM_MUTABLE_PAUSED | GST_PARAM_MUTABLE_READY)

static GHashTable *reader_map = NULL;

/**
    @brief Default param reader

    This reader is called when no suitable reader is found for a given param type.
*/
static void gst_default_reader_fill_type(GParamSpec *pspec,
                                         GValue *value,
                                         GstStructure *dictionary)
{
    g_return_if_fail(G_IS_PARAM_SPEC(pspec));
    g_return_if_fail(G_IS_VALUE(value));

    gst_dictionary_set_string(dictionary, KEY_TYPE,
                              g_strdup_printf("Unknown type %ld \"%s\"",
                                              (glong)pspec->value_type,
                                              g_type_name(pspec->value_type)));
}

/**
 *  @brief Initializer for type readers.
 *  
 *  Should be called internally and only once. It will handle registring all the built-in types,
 *  as well as registring any built-in content parser
 */
void _gst_init_type_readers(void)
{
    reader_map = g_hash_table_new(g_direct_hash, g_direct_equal);

    gst_type_reader_register(G_TYPE_PARAM_BOOLEAN, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_BOXED, gst_boxed_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_DOUBLE, gst_double_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_ENUM, gst_enum_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_FLAGS, gst_flags_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_FLOAT, gst_float_type_reader_fill_type);
    gst_type_reader_register(GST_TYPE_PARAM_FRACTION, gst_fraction_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_INT, gst_int_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_INT64, gst_int64_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_LONG, gst_long_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_OBJECT, gst_object_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_POINTER, gst_pointer_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_STRING, gst_string_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_UINT, gst_uint_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_UINT64, gst_uint64_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_ULONG, gst_ulong_type_reader_fill_type);

    gst_boxed_content_reader_register(GST_TYPE_CAPS, gst_caps_content_reader_parse);
    gst_boxed_content_reader_register(GST_TYPE_STRUCTURE, gst_structure_content_reader_parse);
}

/**
 *  @brief Registers a type reader function for a GParamSpec type
 *
 *  Note that a reader can be registered only once. Registering a reader for existing
 *  type will issue a warning and do nothing.
 *
 *  @param[in] pspec_type A GType of GParamSpec to register a reader function for.
 *  @param[in] read_func A reader function for this type_reader.
 *
 *  @return TRUE if register succeeded, otherwise FALSE
 */
gboolean gst_type_reader_register(GType pspec_type,
                                  GstReadTypeFunc read_func)
{
    if (G_UNLIKELY(NULL == reader_map))
    {
        _gst_init_type_readers();
    }

    g_return_val_if_fail(g_type_is_a(pspec_type, G_TYPE_PARAM), FALSE);

    if (!g_hash_table_contains(reader_map, GINT_TO_POINTER(pspec_type)))
    {
        g_hash_table_insert(reader_map, GINT_TO_POINTER(pspec_type), (gpointer)read_func);
        return TRUE;
    }
    else
    {
        g_warning("Trying to register a reader for existing type %s. Not registering",
                  g_type_name(pspec_type));
        return FALSE;
    }
}

static void read_flags(GParamSpec *param, GstStructure *dictionary)
{
    GArray *flags_array = g_array_new(FALSE, FALSE, sizeof(GValue));
    g_array_set_clear_func(flags_array, (GDestroyNotify)g_value_unset);

    if (param->flags & G_PARAM_READABLE)
    {
        g_array_add_static_string(flags_array, "readable");
    }
    if (param->flags & G_PARAM_WRITABLE)
    {
        g_array_add_static_string(flags_array, "writable");
    }
    if (param->flags & G_PARAM_DEPRECATED)
    {
        g_array_add_static_string(flags_array, "deprecated");
    }
    if (param->flags & GST_PARAM_CONTROLLABLE)
    {
        g_array_add_static_string(flags_array, "controllable");
    }
    if (param->flags & GST_PARAM_MUTABLE_PLAYING)
    {
        g_array_add_static_string(flags_array,
                                  "changeable in NULL, READY, PAUSED or PLAYING state");
    }
    else if (param->flags & GST_PARAM_MUTABLE_PAUSED)
    {
        g_array_add_static_string(flags_array,
                                  "changeable only in NULL, READY or PAUSED state");
    }
    else if (param->flags & GST_PARAM_MUTABLE_READY)
    {
        g_array_add_static_string(flags_array,
                                  "changeable only in NULL or READY state");
    }
    if (param->flags & ~KNOWN_PARAM_FLAGS)
    {
        g_array_add_string(flags_array,
                           g_strdup_printf("0x%0x", param->flags & ~KNOWN_PARAM_FLAGS));
    }

    gst_dictionary_set_array(dictionary, KEY_FLAGS, flags_array);
}

/**
 *  @brief Parses a parameter, using a GParamSpec and GValue, and fills the GstStructure
 *  with string describing it.
 *
 *  The function looks for registered redaer functions, and if a suitable function is available
 *  it is invoked. If not, a default function is invoked.
 *
 *  @param pspec A GParamSpec describing the parameter
 *  @param value A GValue holding a default value for the parameter
 *  @param dictionary Dictionary to fill with parsed data
 */
void gst_type_reader_fill_type(GParamSpec *pspec,
                               const GValue *value,
                               GstStructure *dictionary)
{
    GValue tmp_value = G_VALUE_INIT;
    GstReadTypeFunc read_func = NULL;

    g_return_if_fail(G_IS_PARAM_SPEC(pspec));
    g_return_if_fail(GST_IS_STRUCTURE(dictionary));

    gst_value_init_and_copy(&tmp_value, value);
    g_return_if_fail(G_IS_VALUE(&tmp_value));

    if (G_UNLIKELY(NULL == reader_map))
    {
        _gst_init_type_readers();
    }

    gst_dictionary_set_string(dictionary, KEY_NAME,
                              g_strdup_printf("%s: %s", pspec->name,
                                              g_param_spec_get_blurb(pspec)));

    read_flags(pspec, dictionary);

    read_func = (GstReadTypeFunc)g_hash_table_lookup(
        reader_map, GINT_TO_POINTER(G_PARAM_SPEC_TYPE(pspec)));
    if (read_func)
    {
        read_func(pspec, &tmp_value, dictionary);
    }
    else
    {
        gst_default_reader_fill_type(pspec, &tmp_value, dictionary);
    }

    if (G_IS_VALUE(&tmp_value))
    {
        g_value_unset(&tmp_value);
    }
}
