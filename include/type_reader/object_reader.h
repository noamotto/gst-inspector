#ifndef OBJECT_READER_H
#define OBJECT_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_OBJECT_TYPE_READER   (gst_object_type_reader_get_type())

typedef struct _GstObjectTypeReader
{
    GstTypeReader type_reader;
}GstObjectTypeReader;

GType gst_object_type_reader_get_type();

G_END_DECLS

#endif //OBJECT_READER_H
