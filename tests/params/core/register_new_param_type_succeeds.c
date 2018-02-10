#include "type_reader/type_reader.h"
#include "gfakeparam.h"

static void fake_read_function(GParamSpec *pspec,
                               GValue *value,
                               GstStructure *dictionary)
{
    (void)(pspec);
    (void)(value);
    (void)(dictionary);
    //Does nothing
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    g_assert_true(gst_type_reader_register(G_TYPE_PARAM_FAKE, fake_read_function));

    return 0;
}
