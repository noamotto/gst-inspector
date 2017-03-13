#ifndef STRING_READER_H
#define STRING_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_STRING_TYPE_READER   (gst_string_type_reader_get_type())

typedef struct _GstStringTypeReader
{
    GstTypeReader type_reader;
}GstStringTypeReader;

GType gst_string_type_reader_get_type();

G_END_DECLS

#endif //STRING_READER_H
