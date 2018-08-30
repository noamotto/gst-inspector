/**
 *  @file gstdictionary.cpp
 *  @brief Utilities For GstInspector's dictionaries
 */
#include "gstdictionary.h"

/**
 *  @brief Adds a static string into an array
 *
 *  The static counterpart of g_array_add_string. It assumes the string is 
 *  static and does not copy it for effeciency.
 *
 *  @param array Array to add to
 *  @param string A string to add. The function assumes string is static, and does
 *  not take ownership
 *  @returns The array
 */
GArray *g_array_add_static_string(GArray *array, const gchar *string)
{
    GValue val = G_VALUE_INIT;
    g_value_init(&val, G_TYPE_STRING);
    g_value_set_static_string(&val, string);

    return g_array_append_val(array, val);
}

/**
 *  @brief Adds a static string into an array
 *
 *  The function adds a GValue holding a given string to a GArray, allowing simpler API for 
 *
 *  @param array Array to add to
 *  @param string A string to add. The function takes ownership on the string
 *  @returns The array
 */
GArray * g_array_add_string(GArray *array, gchar *string)
{
    GValue val = G_VALUE_INIT;
    g_value_init(&val, G_TYPE_STRING);
    g_value_take_string(&val, string);

    return g_array_append_val(array, val);
}

/**
 *  @brief Sets a string into the dictionary
 *
 *  This function practically works as setting a GValue into a GstStructure.
 *  For more information, look for gst_structure_take_value
 *
 *  @param dictionary A GstStructure dictionary
 *  @param field_name The field name to set
 *  @param string A string to set. The function takes ownership on the string
 */
void gst_dictionary_set_string(GstStructure *dictionary, const gchar *field_name, gchar *string)
{
    GValue val = G_VALUE_INIT;

    g_return_if_fail(GST_IS_STRUCTURE(dictionary));
    g_return_if_fail(NULL != field_name);
    g_return_if_fail(NULL != string);

    g_value_init(&val, G_TYPE_STRING);
    g_value_take_string(&val, string);
    gst_structure_take_value(dictionary, field_name, &val);
}

/**
 *  @brief Sets a static string into the dictionary.
 * 
 *  The static counterpart of gst_dictionary_set_string. It assumes the string 
 *  is static and does not copy it for effeciency.
 * 
 *  @param dictionary A GstStructure dictionary
 *  @param field_name The field name to set
 *  @param string A string to set. The function assumes string is static, and does
 *  not take ownership
 */
void gst_dictionary_set_static_string(GstStructure *dictionary, const gchar *field_name,
                                      const gchar *string)
{
    GValue val = G_VALUE_INIT;

    g_return_if_fail(GST_IS_STRUCTURE(dictionary));
    g_return_if_fail(NULL != field_name);
    g_return_if_fail(NULL != string);

    g_value_init(&val, G_TYPE_STRING);
    g_value_set_static_string(&val, string);
    gst_structure_take_value(dictionary, field_name, &val);
}

/**
 *  @brief Sets an array into the dictionary
 *
 *  This function practically works as setting a GValue into a GstStructure.
 *  For more information, look for gst_structure_take_value
 *
 *  @param dictionary A GstStructure dictionary
 *  @param field_name The field name to set
 *  @param array An array to set. The function takes ownership on the array
 */
void gst_dictionary_set_array(GstStructure *dictionary, const gchar *field_name, GArray *array)
{
    GValue val = G_VALUE_INIT;

    g_return_if_fail(GST_IS_STRUCTURE(dictionary));
    g_return_if_fail(NULL != array);
    g_return_if_fail(NULL != field_name);

    g_value_init(&val, G_TYPE_ARRAY);
    g_value_take_boxed(&val, array);
    gst_structure_take_value(dictionary, field_name, &val);
}

/**
 *  @brief Sets a sub-dictionary into the dictionary
 *
 *  This function practically works as setting a GValue into a GstStructure.
 *  For more information, look for gst_structure_take_value
 *
 *  @param dictionary A GstStructure dictionary
 *  @param field_name The field name to set
 *  @param sub_dictionary A sub-dictionary to set. The function takes ownership
 *  on the sub-dictionary
 */
void gst_dictionary_set_sub_dictionary(GstStructure *dictionary, const gchar *field_name,
                                       GstStructure *sub_dictionary)
{
    GValue val = G_VALUE_INIT;

    g_return_if_fail(GST_IS_STRUCTURE(dictionary));
    g_return_if_fail(NULL != field_name);
    g_return_if_fail(GST_IS_STRUCTURE(sub_dictionary));

    g_value_init(&val, GST_TYPE_STRUCTURE);
    g_value_take_boxed(&val, sub_dictionary);
    gst_structure_take_value(dictionary, field_name, &val);
}

/**
 *  @brief Retrieves an array field from a dictionary
 *
 *  @param dictionary A GstStructure dictionary
 *  @param field_name The name of the field to retrieve
 *  
 *  @returns The array field in the dictionary, or NULL if the given field does
 *  not exist or isn't an array.
 */
GArray *gst_dictionary_get_array(GstStructure *dictionary, const gchar *field_name)
{
    const GValue *val;

    g_return_val_if_fail(GST_IS_STRUCTURE(dictionary), NULL);
    g_return_val_if_fail(NULL != field_name, NULL);

    val = gst_structure_get_value(dictionary, field_name);
    if (G_TYPE_ARRAY != val->g_type)
    {
        return NULL;
    }
    return g_value_get_boxed(val);
}

/**
    @brief Retrieves a sub-dictionary from a dictionary

    @param dictionary A GstStructure dictionary
    @param field_name The name of the field to retrieve
    
    @returns The sub-dictionary field in the dictionary, or NULL if the given 
    field does not exist or isn't a sub-dictionary (GstStructure).
*/
GstStructure *gst_dictionary_get_sub_dictionary(GstStructure *dictionary, const gchar *field_name)
{
    const GValue *val;

    g_return_val_if_fail(GST_IS_STRUCTURE(dictionary), NULL);
    g_return_val_if_fail(NULL != field_name, NULL);

    val = gst_structure_get_value(dictionary, field_name);
    if (GST_TYPE_STRUCTURE != val->g_type)
    {
        return NULL;
    }
    return g_value_get_boxed(val);
}
