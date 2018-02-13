#include "gstdictionary.h"

#define FIRST_STR ("hello")
#define OTHER_STR ("bye")
#define TEST_PARAM ("test")


int main()
{
    GstStructure *dictionary =
        gst_structure_new("dict",
                          TEST_PARAM, G_TYPE_STRING, FIRST_STR, NULL);

    // Test dictionary string getting
    g_assert_cmpstr(FIRST_STR, ==, gst_dictionary_get_string(dictionary, TEST_PARAM));

    // Test dictionary string setting
    gst_dictionary_set_string(dictionary, TEST_PARAM, g_strdup_printf(OTHER_STR));
    g_assert_cmpstr(OTHER_STR, ==, gst_dictionary_get_string(dictionary, TEST_PARAM));
    
    // Test dictionary static string setting
    gst_dictionary_set_static_string(dictionary, TEST_PARAM, FIRST_STR);
    g_assert_cmpstr(FIRST_STR, ==, gst_dictionary_get_string(dictionary, TEST_PARAM));
}
