#include "gstinspector.h"

#define ERROR_MESSAGE_NAME ("Error")
#define DESCRIPTION_KEY ("Description")
#define ERROR_MESSAGE ("Object name cannot be NULL")

int main(int argc, char *argv[])
{
    GstStructure *data;
    const gchar *error_message;

    gst_init(&argc, &argv);

    data = gst_inspector_inspect_by_name(NULL);

    g_assert_cmpstr(ERROR_MESSAGE_NAME, ==, gst_structure_get_name(data));

    error_message = gst_dictionary_get_string(data, DESCRIPTION_KEY);
    g_assert_cmpstr(error_message, ==, ERROR_MESSAGE);

    return 0;
}
