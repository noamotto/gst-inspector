/**
*   GstDictionary - an implementation of dictionary for gst-inspector
*   
*   Set of utility functions that allow simple assignment and retrieval of supported types. 
*   Where possible, only a macro is used to call GstStructure's API
*/

#ifndef GST_DICTIONARY_H
#define GST_DICTIONARY_H

#include "common.h"

G_BEGIN_DECLS

void gst_dictionary_set_string(GstStructure *dictionary, const gchar *field_name, gchar *string);
void gst_dictionary_set_static_string(GstStructure *dictionary, const gchar *field_name,
                                      const gchar *string);
void gst_dictionary_set_array(GstStructure *dictionary, const gchar *field_name, GArray *array);
void gst_dictionary_set_sub_dictionary(GstStructure *dictionary, const gchar *field_name,
                                       GstStructure *sub_dictionary);

#define gst_dictionary_get_string gst_structure_get_string
GArray *gst_dictionary_get_array(GstStructure *dictionary, const gchar *field_name);
GstStructure *gst_dictionary_get_sub_dictionary(GstStructure *dictionary, const gchar *field_name);

G_END_DECLS

#endif //GST_DICTIONARY_H
