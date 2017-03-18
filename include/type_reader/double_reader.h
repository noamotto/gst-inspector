#ifndef DOUBLE_READER_H
#define DOUBLE_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_DOUBLE_TYPE_READER   (gst_double_type_reader_get_type())

typedef struct _GstDoubleTypeReader
{
    GstTypeReader type_reader;
}GstDoubleTypeReader;

GType gst_double_type_reader_get_type();

G_END_DECLS

#endif //DOUBLE_READER_H
