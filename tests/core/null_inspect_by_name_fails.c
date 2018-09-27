#include "gstinspector.h"

#define DESCRIPTION_KEY ("Error")
#define ERROR_MESSAGE ("Object name cannot be NULL")

int main(int argc, char *argv[])
{
    GstStructure *data;
    const gchar *error_message;

    gst_init(&argc, &argv);

    data = gst_inspector_inspect_by_name(NULL);

    error_message = gst_dictionary_get_string(data, DESCRIPTION_KEY);
    g_assert_cmpstr(error_message, ==, ERROR_MESSAGE);

    return 0;
}
