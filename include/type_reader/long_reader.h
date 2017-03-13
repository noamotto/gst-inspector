#ifndef LONG_READER_H
#define LONG_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_LONG_TYPE_READER   (gst_long_type_reader_get_type())

typedef struct _GstLongTypeReader
{
    GstTypeReader type_reader;
}GstLongTypeReader;

GType gst_long_type_reader_get_type();

G_END_DECLS

#endif //LONG_READER_H
