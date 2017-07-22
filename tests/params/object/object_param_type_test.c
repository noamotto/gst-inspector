#include "type_reader/type_reader.h"
#include "gfakeobject.h"

#define TEST_STRING     ("Test")

static void fake_read_function(GObject *object, GstStructure * dictionary)
{
    GValue key_value = G_VALUE_INIT;

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, TEST_STRING);
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}

int main(int argc, char *argv[])
{
    GParamSpec *object_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary = NULL;
    gchar *type_string = g_strdup_printf("Object of type \"%s\"",
                                                    g_type_name(G_TYPE_FAKE_OBJECT));

    gst_init(&argc, &argv);

    gst_object_content_reader_register(G_TYPE_FAKE_OBJECT, fake_read_function);
    object_spec = g_param_spec_object("test", "Test param",
                                    "Test param", G_TYPE_FAKE_OBJECT, G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(object_spec);

    dictionary = gst_type_reader_fill_type(object_spec, value);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_NAME), ==,"test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_TYPE),==, type_string);

    g_assert_false(gst_structure_has_field(dictionary, KEY_VALUE));

    g_free(type_string);
    return 0;
}
