#ifndef ULONG_READER_H
#define ULONG_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_ULONG_TYPE_READER   (gst_ulong_type_reader_get_type())

typedef struct _GstULongTypeReader
{
    GstTypeReader type_reader;
}GstULongTypeReader;

GType gst_ulong_type_reader_get_type();

G_END_DECLS

#endif //ULONG_READER_H
