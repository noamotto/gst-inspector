#ifndef BOOL_READER_H
#define BOOL_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_BOOL_TYPE_READER   (gst_bool_type_reader_get_type())

typedef struct _GstBoolTypeReader
{
    GstTypeReader type_reader;
}GstBoolTypeReader;

GType gst_bool_type_reader_get_type();

G_END_DECLS

#endif //BOOL_READER_H
