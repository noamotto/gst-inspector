#ifndef TYPE_READER_H
#define TYPE_READER_H

#include "common.h"

G_BEGIN_DECLS

typedef struct _GstTypeReader GstTypeReader;
typedef struct _GstTypeReaderInfo GstTypeReaderInfo;

typedef void (*GstFillTypeFunc)(const GParamSpec *pspec,
                                const GValue *value);

typedef void (*GstTypeReaderInit)(GstTypeReader *type_reader);

typedef void (*GstTypeReaderCopy)(GstTypeReader *type_reader,
                                  GstTypeReader *other_reader);

typedef void (*GstTypeReaderFree)(GstTypeReader *type_reader);

struct _GstTypeReader
{
    GType reader_type;

    GstFillTypeFunc fill_type;
};

struct _GstTypeReaderInfo
{
    gsize reader_size;
    GstTypeReaderInit init_func;
    GstTypeReaderCopy copy_func;
    GstFillTypeFunc fill_func;
    GstTypeReaderFree free_func;
};

GstTypeReader *gst_type_reader_new(GType param_type);
GstTypeReader *gst_type_reader_copy(const GstTypeReader *type_reader);
void gst_type_reader_free(GstTypeReader *type_reader);

void gst_type_reader_register(GType param_type, GstTypeReaderInfo *reader_info);

void gst_type_reader_fill_type(
    GstTypeReader *type_reader,
    const GParamSpec *pspec,
    const GValue *value);

#define GST_DEFINE_TYPE_READER                                              \
    (TypeName, type_name, fill_func, init_func, copy_func, free_func)       \
        G_DEFINE_BOXED_TYPE_WITH_CODE(TypeName, type_name,                  \
                                      (GBoxedCopyFunc)gst_type_reader_copy, \
                                      (GBoxedFreeFunc)gst_type_reader_free) \
    {                                                                       \
        GstTypeReaderInfo info =                                            \
            {                                                               \
                sizeof(TypeName),                                           \
                (GstTypeReaderInit)init_func,                               \
                (GstTypeReaderCopy)copy_func,                               \
                (GstFillTypeFunc)fill_func,                                 \
                (GstTypeReaderFree)free_func,                               \
            };                                                              \
        gst_type_reader_register(g_define_type_id, &info);                  \
    })

G_END_DECLS

#endif //TYPE_READER_H
