#ifndef FLAGS_READER_H
#define FLAGS_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_FLAGS_TYPE_READER   (gst_flags_type_reader_get_type())

typedef struct _GstFlagsTypeReader
{
    GstTypeReader type_reader;
}GstFlagsTypeReader;

GType gst_flags_type_reader_get_type();

G_END_DECLS

#endif //FLAGS_READER_H
