#include "type_reader/type_reader.h"
#include "gfakeboxed.h"

int main(int argc, char *argv[])
{
    GParamSpec *boxed_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary = NULL;
    gchar *type_string = g_strdup_printf("Boxed pointer of type \"%s\"",
                                         g_type_name(G_TYPE_FAKE_BOXED));

    gst_init(&argc, &argv);

    boxed_spec = g_param_spec_boxed("test", "Test param",
                                   "Test param", G_TYPE_FAKE_BOXED, G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(boxed_spec);
    dictionary = gst_structure_new_empty("dictionary");

    gst_type_reader_fill_type(boxed_spec, (GValue * const)value, dictionary);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_NAME), "test: Test param"),
                         1);

    g_return_val_if_fail(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING), 1);
    g_return_val_if_fail(g_str_equal(
                             gst_structure_get_string(dictionary, KEY_TYPE), type_string),
                         1);

    g_return_val_if_fail(!gst_structure_has_field(dictionary, KEY_VALUE), 1);

    g_free(type_string);
    return 0;
}
