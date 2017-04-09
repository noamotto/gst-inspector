#include "type_reader/type_reader_priv.h"

static GHashTable *reader_map = NULL;

static void gst_default_reader_fill_type(const GParamSpec *pspec,
                                  const GValue *value,
                                  GstStructure *const dictionary)
{
    GValue key_value = G_VALUE_INIT;
    (void)(value);

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("Unknown type %ld \"%s\"", "",
                                                    (glong)pspec->value_type, g_type_name(pspec->value_type)));

    gst_structure_take_value(dictionary, KEY_TYPE, &key_value);
}

/**
    @brief Initializer for type readers.
    
    Should be called internally and only once. It will handle registring all the built-in types,
    as well as registring any built-in content parser
*/
void _gst_init_type_readers()
{
    reader_map = g_hash_table_new(g_direct_hash, g_direct_equal);

    gst_type_reader_register(G_TYPE_PARAM_BOOLEAN, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_BOXED, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_DOUBLE, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_ENUM, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_FLAGS, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_FLOAT, gst_bool_type_reader_fill_type);
    gst_type_reader_register(GST_TYPE_PARAM_FRACTION, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_INT, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_INT64, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_LONG, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_OBJECT, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_POINTER, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_STRING, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_UINT, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_UINT64, gst_bool_type_reader_fill_type);
    gst_type_reader_register(G_TYPE_PARAM_ULONG, gst_bool_type_reader_fill_type);

    gst_boxed_content_reader_register(GST_TYPE_CAPS, gst_caps_content_reader_parse);
    gst_boxed_content_reader_register(GST_TYPE_STRUCTURE, gst_structure_content_reader_parse);
}

/**
    @brief Registers a type reader function for a GParamSpec type

    Note that a reader can be registered only once. Registering a reader for existing
    type will issue a warning and do nothing.

    @param[in] pspec_type A GType of GParamSpec to register a reader function for.
    @param[in] read_func A reader function for this type_reader.

    @return TRUE if register succeeded, otherwise FALSE
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

/**
    @brief Parses a parameter, using a GParamSpec and GValue, and fills the GstStructure
    with string describing it.

    The function looks for registered redaer functions, and if a suitable function is available
    it is invoked. If not, a default function is invoked.

    @param pspec A GParamSpec describing the parameter
    @param value A GValue holding a default value for the parameter
    @param dictionary A dictionary to fill
*/
void gst_type_reader_fill_type(
    GParamSpec *const pspec,
    GValue *const value,
    GstStructure *const dictionary)
{
   GValue key_value = G_VALUE_INIT;
   GstReadTypeFunc read_func = NULL;

    g_return_if_fail(NULL != pspec);
    g_return_if_fail(NULL != value);
    g_return_if_fail(NULL != dictionary);

    if (G_UNLIKELY(NULL == reader_map))
    {
        _gst_init_type_readers();
    }

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, g_strdup_printf("%s: %s", pspec->name,
                                                    g_param_spec_get_blurb(pspec)));
    gst_structure_take_value(dictionary, KEY_NAME, &key_value);

    read_func = (GstReadTypeFunc)g_hash_table_lookup(
        reader_map, GINT_TO_POINTER(G_PARAM_SPEC_TYPE(pspec)));
    if (read_func)
    {
        read_func(pspec, value, dictionary);
    }
    else
    {
        gst_default_reader_fill_type(pspec, value, dictionary);
    }
}
