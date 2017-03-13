#ifndef INT64_READER_H
#define INT64_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_INT64_TYPE_READER   (gst_int64_type_reader_get_type())

typedef struct _GstInt64TypeReader
{
    GstTypeReader type_reader;
}GstInt64TypeReader;

GType gst_int64_type_reader_get_type();

G_END_DECLS

#endif //INT64_READER_H
