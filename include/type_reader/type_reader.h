#ifndef TYPE_READER_H
#define TYPE_READER_H

#include "gstdictionary.h"

G_BEGIN_DECLS

typedef void (*GstReadTypeFunc)(GParamSpec * pspec,
                                GValue * value,
                                GstStructure * dictionary);

typedef void (*GstBoxedContentReaderFunc)(GValue * boxed,
                                          GstStructure * dictionary);

typedef void (*GstObjectContentReaderFunc)(GObject *object,
                                           GstStructure * dictionary);

GST_INSPECTOR_API gboolean gst_type_reader_register(GType pspec_type,
                                                    GstReadTypeFunc read_func);

GST_INSPECTOR_API gboolean
gst_boxed_content_reader_register(GType boxed_type,
                                  GstBoxedContentReaderFunc read_func);

GST_INSPECTOR_API gboolean
gst_object_content_reader_register(GType object_type,
                                   GstObjectContentReaderFunc read_func);

GST_INSPECTOR_API GstStructure * gst_type_reader_fill_type(GParamSpec * pspec,
                                                 const GValue * value);

G_END_DECLS

#endif //TYPE_READER_H
