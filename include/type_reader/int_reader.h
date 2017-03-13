#ifndef INT_READER_H
#define INT_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_INT_TYPE_READER   (gst_int_type_reader_get_type())

typedef struct _GstIntTypeReader
{
    GstTypeReader type_reader;
}GstIntTypeReader;

GType gst_int_type_reader_get_type();

G_END_DECLS

#endif //INT_READER_H
