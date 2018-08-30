#include "type_reader/type_reader.h"
#include "gfakeparam.h"

gboolean read_called = FALSE;

static void fake_read_function(GParamSpec *pspec,
                               GValue *value,
                               GstStructure *dictionary)
{
    read_called = TRUE;
}

int main(int argc, char *argv[])
{
    GParamSpec *fake_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary;
    gst_init(&argc, &argv);

    dictionary = gst_structure_new_empty("test");
    gst_type_reader_register(G_TYPE_PARAM_FAKE, fake_read_function);
    
    fake_spec = g_param_spec_fake("test", "Test param", "Test param", G_PARAM_READWRITE);
    value = g_param_spec_get_default_value(fake_spec);
    gst_type_reader_fill_type(fake_spec, value, dictionary);

    g_assert_true(read_called);

    return 0;
}
