#include "type_reader/type_reader.h"

//General GTypeReader declarations

static GHashTable *reader_map = NULL;

/**
    @brief A sttructure holding all information about a type reader.
    Serves as type inforamtion
*/
struct _GstTypeReaderInfo
{
    gsize reader_size;
    GstTypeReaderInit init_func;
    GstTypeReaderCopy copy_func;
    GstFillTypeFunc fill_func;
    GstTypeReaderFree free_func;
};

/**
    @brief A default reader. This class is the fallback for 
*/
typedef struct _GstDefaultReader
{
    GstTypeReader type_reader;
} GstDefaultReader;

G_DEFINE_BOXED_TYPE(GstDefaultReader,
                    gst_default_reader,
                    gst_type_reader_copy,
                    gst_type_reader_free);

GType gst_default_reader_type = 0;

void gst_default_reader_fill_type(const GParamSpec *pspec,
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
    @brief Initializer for type readers. Should be called once for a program run, 
    to provide the type handling and registering the default handler
*/
void _gst_type_reader_init()
{
    g_hash_table_new(g_direct_hash, g_direct_equal);

    gst_default_reader_type = gst_default_reader_get_type();
}

GstTypeReader *gst_type_reader_new(GType reader_type)
{
    GstTypeReader *type_reader = NULL;
    GstTypeReaderInfo *reader_info = NULL;

    if (G_UNLIKELY(NULL == reader_map))
    {
        _gst_type_reader_init();
    }

    reader_info = (GstTypeReaderInfo *)g_hash_table_lookup(reader_map,
                                                           GINT_TO_POINTER(reader_type));

    if (NULL == reader_info)
    {
        type_reader = (GstTypeReader *)g_slice_alloc(sizeof(GstDefaultReader));
        type_reader->reader_type = gst_default_reader_type;
        type_reader->fill_type = gst_default_reader_fill_type;
    }
    else
    {
        type_reader = (GstTypeReader *)g_slice_alloc(reader_info->reader_size);
        type_reader->reader_type = reader_type;
        type_reader->fill_type = reader_info->fill_func;

        if (reader_info->init_func)
        {
            reader_info->init_func(type_reader);
        }
    }

    return NULL;
}

GstTypeReader *gst_type_reader_copy(const GstTypeReader *type_reader)
{
    GstTypeReaderInfo *reader_info = NULL;
    GstTypeReader *other_reader = NULL;

    g_return_val_if_fail(NULL != type_reader, NULL);
    if (G_UNLIKELY(NULL == reader_map))
    {
        _gst_type_reader_init();
    }

    reader_info = (GstTypeReaderInfo *)g_hash_table_lookup(reader_map,
                                                           GINT_TO_POINTER(type_reader->reader_type));

    g_return_val_if_fail(NULL != reader_info, NULL);

    other_reader = (GstTypeReader *)g_slice_alloc(reader_info->reader_size);
    other_reader->reader_type = type_reader->reader_type;
    other_reader->fill_type = reader_info->fill_func;

    if (NULL != reader_info->copy_func)
    {
        reader_info->copy_func(type_reader, other_reader);
    }

    return other_reader;
}

void gst_type_reader_free(GstTypeReader *type_reader)
{
    GstTypeReaderInfo *reader_info = NULL;

    g_return_if_fail(NULL != type_reader);
    if (G_UNLIKELY(NULL == reader_map))
    {
        _gst_type_reader_init();
    }

    reader_info = (GstTypeReaderInfo *)g_hash_table_lookup(reader_map,
                                                           GINT_TO_POINTER(type_reader->reader_type));

    g_return_if_fail(NULL != reader_info);

    if (NULL != reader_info->free_func)
    {
        reader_info->free_func(type_reader);
    }

    g_slice_free1(reader_info->reader_size, type_reader);
}

void gst_type_reader_register(GType reader_type,
                              gsize reader_size,
                              GstTypeReaderInit init_func,
                              GstTypeReaderCopy copy_func,
                              GstFillTypeFunc fill_func,
                              GstTypeReaderFree free_func)
{
    if (G_UNLIKELY(NULL == reader_map))
    {
        _gst_type_reader_init();
    }

    if (!g_hash_table_contains(reader_map, GINT_TO_POINTER(reader_type)))
    {
        GstTypeReaderInfo *info = (GstTypeReaderInfo *)g_malloc(sizeof(GstTypeReader));

        info->reader_size = reader_size;
        info->fill_func = fill_func;
        info->init_func = init_func;
        info->copy_func = copy_func;
        info->free_func = free_func;
        g_hash_table_insert(reader_map, GINT_TO_POINTER(reader_type), info);
    }
    else
    {
        g_warning("Trying to register a reader for existing type %s. Not registering",
                  g_type_name(reader_type));
    }
}

void gst_type_reader_fill_type(
    GstTypeReader *type_reader,
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary)
{
    g_return_if_fail(NULL != type_reader);
    g_return_if_fail(NULL != type_reader->fill_type);

    type_reader->fill_type(pspec, value, dictionary);
}
