#ifndef FRACTION_READER_H
#define FRACTION_READER_H

#include "type_reader/type_reader.h"

G_BEGIN_DECLS

#define GST_TYPE_FRACTION_TYPE_READER   (gst_fraction_type_reader_get_type())

typedef struct _GstFractionTypeReader
{
    GstTypeReader type_reader;
}GstFractionTypeReader;

GType gst_fraction_type_reader_get_type();

G_END_DECLS

#endif //FRACTION_READER_H
