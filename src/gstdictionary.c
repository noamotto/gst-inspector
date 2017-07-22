#include "gstdictionary.h"

/**
    @brief Sets a string into the dictionary

    This function practically works as setting a GValue into a GstStructure.
    For more information, look for gst_structure_take_value

    @param dictionary A GstStructure dictionary
    @param field_name The field name to set
    @param string A string to set. The function takes ownership on the string
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
    @brief Sets a string into the dictionary.

    This function practically works as setting a GValue into a GstStructure.
    For more information, look for gst_structure_take_value

    @param dictionary A GstStructure dictionary
    @param field_name The field name to set
    @param string A string to set. The function assumes string is static, and does
     not take ownership
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
    @brief Sets an array into the dictionary

    This function practically works as setting a GValue into a GstStructure.
    For more information, look for gst_structure_take_value

    @param dictionary A GstStructure dictionary
    @param field_name The field name to set
    @param array An array to set. The function takes ownership on the array
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
    @brief Sets a sub-dictionary into the dictionary

    This function practically works as setting a GValue into a GstStructure.
    For more information, look for gst_structure_take_value

    @param dictionary A GstStructure dictionary
    @param field_name The field name to set
    @param sub_dictionary A sub-dictionary to set. The function takes ownership
    on the sub-dictionary
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
