#include <gstinspector.h>

static gint tabs = 0;

void print_usage(void);
void print_field(const gchar *name, const GValue *field);
void print_array(const GValue *array);
void print_dictionary(const GstStructure *dictionary);

void print_usage()
{
    g_print("Usage: simple_inspection <objectname>");
}

void print_field(const gchar *name, const GValue *field)
{
    for (gint i = 0; i < tabs; i++)
    {
        g_print("    ");
    }

    if (name != NULL)
    {
        g_print("%s: %s\n", name, g_value_get_string(field));
    }
    else
    {
        g_print("%s\n", g_value_get_string(field));
    }
}

void print_array(const GValue *array)
{
    guint length = gst_array_get_size(array);

    for (guint i = 0; i < length; i++)
    {
        const GValue *field = gst_array_get_value(array, i);

        if (G_VALUE_HOLDS_STRING(field))
        {
            print_field(NULL, field);
        }
        else if (GST_VALUE_HOLDS_LIST(field))
        {
            ++tabs;
            print_array(field);
            --tabs;
        }
        else
        {
            ++tabs;
            print_dictionary(gst_value_get_structure(field));
            --tabs;
        }
        g_print("\n");
    }
}

void print_dictionary(const GstStructure *dictionary)
{
    gint length = gst_dictionary_get_size(dictionary);

    for (gint i = 0; i < length; i++)
    {
        const gchar *field_name = gst_dictionary_get_nth_name(dictionary, (guint)i);
        const GValue *field = gst_dictionary_get_value(dictionary, field_name);

        if (G_VALUE_HOLDS_STRING(field))
        {
            print_field(field_name, field);
        }
        else if (GST_VALUE_HOLDS_LIST(field))
        {
            g_print("%s:\n", field_name);
            ++tabs;
            print_array(field);
            --tabs;
        }
        else
        {
            g_print("%s:\n", field_name);
            ++tabs;
            print_dictionary(gst_value_get_structure(field));
            --tabs;
        }
        g_print("\n");
    }
}

int main(int argc, char *argv[])
{
    gchar *object_name;
    GstStructure *inspected_data;

    gst_init(&argc, &argv);

    if (argc != 2)
    {
        print_usage();
        return -1;
    }

    object_name = argv[1];

    inspected_data = gst_inspector_inspect_by_name(object_name);
    print_dictionary(inspected_data);

    gst_structure_free(inspected_data);
    return 0;
}
