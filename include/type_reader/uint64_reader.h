#ifndef UINT64_READER_H
#define UINT64_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_UINT64_TYPE_READER   (gst_uint64_type_reader_get_type())

typedef struct _GstUInt64TypeReader
{
    GstTypeReader type_reader;
}GstUInt64TypeReader;

GType gst_uint64_type_reader_get_type();

G_END_DECLS

#endif //UINT64_READER_H
