/**
 *  @file gstdictionary.cpp
 *  @brief Utilities For GstInspector's dictionaries
 */
#include <string.h>
#include "gstdictionary.h"

static inline void
gst_array_prepend_value(GValue *array, GValue *prepend_value)
{
    // Kinda dirty hack, but currently there is no API function handling that
    g_array_prepend_val((GArray *)array->data[0].v_pointer, prepend_value);
    memset(prepend_value, 0, sizeof(GValue));
}

/**
 *  @brief Appends a static string into an array
 *
 *  The function takes the given string as is, so static strings should not be 
 *  passed to it
 *
 *  @param array Array to append to, as a GValue of type GST_TYPE_ARRAY
 *  @param string A string to append. The function takes ownership on the string
 */
void gst_array_append_string(GValue *array, gchar *string)
{
    GValue val = G_VALUE_INIT;
    g_value_init(&val, G_TYPE_STRING);
    g_value_take_string(&val, string);

    gst_value_array_append_and_take_value(array, &val);
}

/**
 *  @brief Appends a static string into an array
 *
 *  Should be used for static strings
 *
 *  @param array Array to append to, as a GValue of type GST_TYPE_ARRAY
 *  @param string A string to append. The function assumes string is static, and does
 *  not take ownership
 */
void gst_array_append_static_string(GValue *array, const gchar *string)
{
    GValue val = G_VALUE_INIT;
    g_value_init(&val, G_TYPE_STRING);
    g_value_set_static_string(&val, string);

    gst_value_array_append_and_take_value(array, &val);
}

/**
 *  @brief Appends a subdictionary into an array
 *
 *  @param array Array to append to, as a GValue of type GST_TYPE_ARRAY
 *  @param dictionary A sub-dictionary to append. The function takes 
 *  ownership on the sub-dictionary
 */
void gst_array_append_subdictionary(GValue *array, GstStructure *dictionary)
{
    GValue val = G_VALUE_INIT;
    g_value_init(&val, GST_TYPE_STRUCTURE);
    g_value_take_boxed(&val, dictionary);

    gst_value_array_append_and_take_value(array, &val);
}

/**
 *  @brief Prepends a static string into an array
 *
 *  The function takes the given string as is, so static strings should not be 
 *  passed to it
 *
 *  @param array Array to prepend to, as a GValue of type GST_TYPE_ARRAY
 *  @param string A string to prepend. The function takes ownership on the string
 */
void gst_array_prepend_string(GValue *array, gchar *string)
{
    GValue val = G_VALUE_INIT;

    g_return_if_fail(G_VALUE_HOLDS_STRING(gst_value_array_get_value(array, 0)));
    g_value_init(&val, G_TYPE_STRING);
    g_value_take_string(&val, string);

    gst_array_prepend_value(array, &val);
}

/**
 *  @brief Prepends a static string into an array
 *
 *  Should be used for static strings
 *
 *  @param array Array to prepend to, as a GValue of type GST_TYPE_ARRAY
 *  @param string A string to prepend. The function assumes string is static, and does
 *  not take ownership
 */
void gst_array_prepend_static_string(GValue *array, const gchar *string)
{
    GValue val = G_VALUE_INIT;

    g_return_if_fail(G_VALUE_HOLDS_STRING(gst_value_array_get_value(array, 0)));
    g_value_init(&val, G_TYPE_STRING);
    g_value_set_static_string(&val, string);

    gst_value_array_append_and_take_value(array, &val);
}

/**
 *  @brief Prepends a subdictionary into an array
 *
 *  @param array Array to prepend to, as a GValue of type GST_TYPE_ARRAY
 *  @param dictionary A sub-dictionary to prepend. The function takes 
 *  ownership on the sub-dictionary
 */
void gst_array_prepend_subdictionary(GValue *array, GstStructure *dictionary)
{
    GValue val = G_VALUE_INIT;

    g_return_if_fail(GST_VALUE_HOLDS_STRUCTURE(gst_value_array_get_value(array, 0)));
    g_value_init(&val, GST_TYPE_STRUCTURE);
    g_value_take_boxed(&val, dictionary);

    gst_value_array_append_and_take_value(array, &val);
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
 *  @param array An array to set, in the form of a GValue holding a GST_TYPE_ARRAY.
 *  The function takes ownership on the array
 */
void gst_dictionary_set_array(GstStructure *dictionary, const gchar *field_name, GValue *array)
{
    g_return_if_fail(GST_IS_STRUCTURE(dictionary));
    g_return_if_fail(GST_VALUE_HOLDS_ARRAY(array));
    g_return_if_fail(NULL != field_name);

    gst_structure_take_value(dictionary, field_name, array);
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
 *  @brief Sets a value into the dictionary
 *
 *  This function can be used as a generic way to add values to a dictionary.
 *  The function makes sure the value type fits the supported dictionary field
 *  types (string, array or dictionary), and handles the insertion. If the value
 *  does not fit, a warning is issued.
 *
 *  @param dictionary A GstStructure dictionary
 *  @param field_name The field name to set
 *  @param value A value to set. The function takes ownership of the value 
 *  regardless to whether it was set or not
 */
void gst_dictionary_set_value(GstStructure *dictionary, const gchar *field_name, GValue *value)
{
    g_return_if_fail(GST_IS_STRUCTURE(dictionary));
    g_return_if_fail(NULL != field_name);
    g_return_if_fail(G_IS_VALUE(value));

    if (G_VALUE_HOLDS(value, G_TYPE_STRING) ||
        G_VALUE_HOLDS(value, GST_TYPE_ARRAY) ||
        G_VALUE_HOLDS(value, GST_TYPE_STRUCTURE))
    {
        gst_structure_take_value(dictionary, field_name, value);
    }
    else
    {
        g_warning("Got invalid type %s for dictionary", G_VALUE_TYPE_NAME(value));
        g_value_unset(value);
    }
}

/**
 *  @brief Retrieves an array field from a dictionary
 *
 *  @param dictionary A GstStructure dictionary
 *  @param field_name The name of the field to retrieve
 *  
 *  @returns The array field in the dictionary, as a GValue holding a GST_TYPE_ARRAY,
 *  or NULL if the given field does not exist or isn't an array.
 */
const GValue *gst_dictionary_get_array(const GstStructure *dictionary, const gchar *field_name)
{
    const GValue *val;

    g_return_val_if_fail(GST_IS_STRUCTURE(dictionary), NULL);
    g_return_val_if_fail(NULL != field_name, NULL);

    val = gst_structure_get_value(dictionary, field_name);
    if (!GST_VALUE_HOLDS_ARRAY(val))
    {
        return NULL;
    }
    return val;
}

/**
    @brief Retrieves a sub-dictionary from a dictionary

    @param dictionary A GstStructure dictionary
    @param field_name The name of the field to retrieve
    
    @returns The sub-dictionary field in the dictionary, or NULL if the given 
    field does not exist or isn't a sub-dictionary (GstStructure).
*/
GstStructure *gst_dictionary_get_sub_dictionary(const GstStructure *dictionary, const gchar *field_name)
{
    const GValue *val;

    g_return_val_if_fail(GST_IS_STRUCTURE(dictionary), NULL);
    g_return_val_if_fail(NULL != field_name, NULL);

    val = gst_structure_get_value(dictionary, field_name);
    if (!GST_VALUE_HOLDS_STRUCTURE(val))
    {
        return NULL;
    }
    return g_value_get_boxed(val);
}
