#ifndef POINTER_READER_H
#define POINTER_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_POINTER_TYPE_READER   (gst_pointer_type_reader_get_type())

typedef struct _GstPointerTypeReader
{
    GstTypeReader type_reader;
}GstPointerTypeReader;

GType gst_pointer_type_reader_get_type();

G_END_DECLS

#endif //POINTER_READER_H
