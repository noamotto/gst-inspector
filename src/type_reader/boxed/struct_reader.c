#include "type_reader/type_reader_priv.h"

void gst_structure_content_reader_parse(GValue *boxed,
                                        GstStructure *dictionary)
{
    const GstStructure *structure;
    GValue fields_array = G_VALUE_INIT;

    g_return_if_fail(GST_VALUE_HOLDS_STRUCTURE(boxed));

    structure = gst_value_get_structure(boxed);
    g_value_init(&fields_array, GST_TYPE_ARRAY);

    if (structure)
    {
        gst_structure_foreach(structure,
                              (GstStructureForeachFunc)parse_field,
                              &fields_array);
    }

    gst_dictionary_set_array(dictionary, KEY_VALUE, &fields_array);
}
