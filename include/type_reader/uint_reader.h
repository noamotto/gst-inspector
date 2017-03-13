#ifndef UINT_READER_H
#define UINT_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_UINT_TYPE_READER   (gst_uint_type_reader_get_type())

typedef struct _GstUIntTypeReader
{
    GstTypeReader type_reader;
}GstUIntTypeReader;

GType gst_uint_type_reader_get_type();

G_END_DECLS

#endif //UINT_READER_H
