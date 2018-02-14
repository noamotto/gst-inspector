/**
*   GstDictionary - an implementation of dictionary for gst-inspector
*   
*   Set of utility functions that allow simple assignment and retrieval of supported types. 
*   Where possible, only a macro is used to call GstStructure's API
*/

#include "common.h"

#ifndef GST_DICTIONARY_H
#define GST_DICTIONARY_H

G_BEGIN_DECLS

GST_INSPECTOR_API void gst_dictionary_set_string(GstStructure *dictionary, const gchar *field_name, gchar *string);
GST_INSPECTOR_API void gst_dictionary_set_static_string(GstStructure *dictionary, const gchar *field_name,
                                      const gchar *string);
GST_INSPECTOR_API void gst_dictionary_set_array(GstStructure *dictionary, const gchar *field_name, GArray *array);
GST_INSPECTOR_API void gst_dictionary_set_sub_dictionary(GstStructure *dictionary, const gchar *field_name,
                                       GstStructure *sub_dictionary);

#define gst_dictionary_get_string gst_structure_get_string
GST_INSPECTOR_API GArray *gst_dictionary_get_array(GstStructure *dictionary, const gchar *field_name);
GST_INSPECTOR_API GstStructure *gst_dictionary_get_sub_dictionary(GstStructure *dictionary, const gchar *field_name);

G_END_DECLS

#endif //GST_DICTIONARY_H
