#include "type_reader/type_reader.h"
#include "gfakeboxed.h"

#define TEST_STRING ("Test")

static void fake_read_function(GValue * boxed, GstStructure * dictionary)
{
    GValue key_value = G_VALUE_INIT;

    g_value_init(&key_value, G_TYPE_STRING);
    g_value_take_string(&key_value, TEST_STRING);
    gst_structure_take_value(dictionary, KEY_VALUE, &key_value);
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    g_assert_true(gst_boxed_content_reader_register(G_TYPE_FAKE_BOXED, fake_read_function));
    g_assert_false(gst_boxed_content_reader_register(G_TYPE_FAKE_BOXED, fake_read_function));

    return 0;
}
