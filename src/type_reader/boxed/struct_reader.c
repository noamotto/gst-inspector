#include "type_reader/type_reader_priv.h"

/**
 *  @addtogroup boxed-readers
 *  @subsection struct-reader Structure boxed content reader
 *  The structure boxed content reader (for GST_TYPE_STRUCTURE) parses the following 
 *  additional fields:
 *  - <b>Default Value</b> - Property's default value. It hold a dictonary of the 
 *      structure's fields, as field name and value.
 */
void gst_structure_content_reader_parse(GValue *boxed,
                                        GstStructure *dictionary)
{
    const GstStructure *structure;
    GValue fields_array = G_VALUE_INIT;

    g_return_if_fail(GST_VALUE_HOLDS_STRUCTURE(boxed));

    structure = gst_value_get_structure(boxed);
    g_value_init(&fields_array, GST_TYPE_LIST);

    if (structure)
    {
        gst_structure_foreach(structure,
                              (GstStructureForeachFunc)parse_field,
                              &fields_array);
    }

    gst_dictionary_set_array(dictionary, KEY_VALUE, &fields_array);
}
