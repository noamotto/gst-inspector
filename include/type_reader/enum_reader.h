#ifndef ENUM_READER_H
#define ENUM_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_ENUM_TYPE_READER   (gst_enum_type_reader_get_type())

typedef struct _GstEnumTypeReader
{
    GstTypeReader type_reader;
}GstEnumTypeReader;

GType gst_enum_type_reader_get_type();

G_END_DECLS

#endif //ENUM_READER_H
