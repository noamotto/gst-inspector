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

// Array helpers
GST_INSPECTOR_API void gst_array_append_static_string(GValue *array,
                                                      const gchar *string);

GST_INSPECTOR_API void gst_array_append_string(GValue *array,
                                               gchar *string);

GST_INSPECTOR_API void gst_array_append_subdictionary(GValue *array,
                                                      GstStructure *dictionary);

GST_INSPECTOR_API void gst_array_prepend_static_string(GValue *array,
                                                       const gchar *string);

GST_INSPECTOR_API void gst_array_prepend_string(GValue *array,
                                                gchar *string);

GST_INSPECTOR_API void gst_array_prepend_subdictionary(GValue *array,
                                                       GstStructure *dictionary);

// Getters
GST_INSPECTOR_API void gst_dictionary_set_string(GstStructure *dictionary,
                                                 const gchar *field_name,
                                                 gchar *string);

GST_INSPECTOR_API void gst_dictionary_set_static_string(GstStructure *dictionary,
                                                        const gchar *field_name,
                                                        const gchar *string);

GST_INSPECTOR_API void gst_dictionary_set_array(GstStructure *dictionary,
                                                const gchar *field_name,
                                                GValue *array);

GST_INSPECTOR_API void gst_dictionary_set_sub_dictionary(GstStructure *dictionary,
                                                         const gchar *field_name,
                                                         GstStructure *sub_dictionary);

GST_INSPECTOR_API void gst_dictionary_set_value(GstStructure *dictionary,
                                                const gchar *field_name,
                                                GValue *value);

// Setters
#define gst_dictionary_get_string gst_structure_get_string
GST_INSPECTOR_API const GValue *gst_dictionary_get_array(const GstStructure *dictionary,
                                                         const gchar *field_name);
GST_INSPECTOR_API GstStructure *gst_dictionary_get_sub_dictionary(const GstStructure *dictionary,
                                                                  const gchar *field_name);

G_END_DECLS

#endif //GST_DICTIONARY_H
