#ifndef TYPE_READER_H
#define TYPE_READER_H

#include "common.h"

G_BEGIN_DECLS

typedef struct _GstTypeReader GstTypeReader;
typedef struct _GstTypeReaderInfo GstTypeReaderInfo;

typedef void (*GstFillTypeFunc)(const GParamSpec *pspec,
                                const GValue *value,
                                GstStructure *const dictionary);

typedef void (*GstTypeReaderInit)(GstTypeReader *type_reader);

typedef void (*GstTypeReaderCopy)(const GstTypeReader *type_reader,
                                  GstTypeReader *other_reader);

typedef void (*GstTypeReaderFree)(GstTypeReader *type_reader);

struct _GstTypeReader
{
    GType reader_type;

    GstFillTypeFunc fill_type;
};

GST_INSPECTOR_API GstTypeReader *gst_type_reader_new(GType reader_type);
GST_INSPECTOR_API GstTypeReader *gst_type_reader_copy(const GstTypeReader *type_reader);
GST_INSPECTOR_API void gst_type_reader_free(GstTypeReader *type_reader);

GST_INSPECTOR_API void gst_type_reader_register(GType reader_type,
                              gsize reader_size,
                              GstTypeReaderInit init_func,
                              GstTypeReaderCopy copy_func,
                              GstFillTypeFunc fill_func,
                              GstTypeReaderFree free_func);

GST_INSPECTOR_API void gst_type_reader_fill_type(
    GstTypeReader *type_reader,
    const GParamSpec *pspec,
    const GValue *value,
    GstStructure *const dictionary);

#define GST_TYPE_DEFAULT_READER (gst_default_reader_type)
GST_INSPECTOR_API extern GType gst_default_reader_type;

#define GST_DEFINE_TYPE_READER_TYPE(TypeName, type_name, fill_func,                      \
                                    init_func, copy_func, free_func)                     \
    G_DEFINE_BOXED_TYPE_WITH_CODE(TypeName, type_name,                                   \
                                  (GBoxedCopyFunc)gst_type_reader_copy,                  \
                                  (GBoxedFreeFunc)gst_type_reader_free,                  \
                                  gst_type_reader_register(g_define_type_id,             \
                                                           sizeof(TypeName),             \
                                                           (GstTypeReaderInit)init_func, \
                                                           (GstTypeReaderCopy)copy_func, \
                                                           (GstFillTypeFunc)fill_func,   \
                                                           (GstTypeReaderFree)free_func))

G_END_DECLS

#endif //TYPE_READER_H
