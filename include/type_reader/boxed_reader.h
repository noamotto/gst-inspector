#ifndef BOXED_READER_H
#define BOXED_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_BOXED_TYPE_READER   (gst_boxed_type_reader_get_type())

typedef struct _GstBoxedTypeReader
{
    GstTypeReader type_reader;
}GstBoxedTypeReader;

GType gst_boxed_type_reader_get_type();

G_END_DECLS

#endif //BOXED_READER_H
