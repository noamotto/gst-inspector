#ifndef TYPE_READER_H
#define TYPE_READER_H

#include "common.h"

G_BEGIN_DECLS

typedef void (*GstReadTypeFunc)(const GParamSpec *pspec,
                                const GValue *value,
                                GstStructure *const dictionary);

typedef void (*GstBoxedContentReaderFunc)(const gpointer boxed,
                                          GstStructure *const dictionary);

typedef void (*GstObjectContentReaderFunc)(const GObject *object,
                                          GstStructure *const dictionary);

GST_INSPECTOR_API gboolean gst_type_reader_register(GType pspec_type,
                                                    GstReadTypeFunc read_func);

GST_INSPECTOR_API gboolean
gst_boxed_content_reader_register(GType boxed_type,
                                  GstBoxedContentReaderFunc read_func);

GST_INSPECTOR_API gboolean
gst_object_content_reader_register(GType object_type,
                                   GstObjectContentReaderFunc read_func);

GST_INSPECTOR_API void gst_type_reader_fill_type(const GParamSpec *pspec,
                                                 const GValue *value,
                                                 GstStructure *const dictionary);

G_END_DECLS

#endif //TYPE_READER_H
