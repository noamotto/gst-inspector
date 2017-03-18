#ifndef FLOAT_READER_H
#define FLOAT_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_FLOAT_TYPE_READER   (gst_float_type_reader_get_type())

typedef struct _GstFloatTypeReader
{
    GstTypeReader type_reader;
}GstFloatTypeReader;

GType gst_float_type_reader_get_type();

G_END_DECLS

#endif //FLOAT_READER_H
