#include "type_reader/type_reader.h"

#define FLAGS_NAME ("GFakeFlags")

typedef enum _GFakeFlags {
    FAKE_VALUE_0 = 1 << 0,
    FAKE_VALUE_1 = 1 << 1,
    FAKE_VALUE_2 = 1 << 2
} GFakeFlags;

#define GST_TYPE_FAKE_FLAGS (gst_fake_flags_get_type())
static GType gst_fake_flags_get_type()
{
    static volatile GType fake_flags_id = 0;

    if (g_once_init_enter(&fake_flags_id))
    {
        GType fake_flags_type;
        static const GFlagsValue fake_values[] = {
            {FAKE_VALUE_0, "value0", "Value 0"},
            {FAKE_VALUE_1, "value1", "Value 1"},
            {FAKE_VALUE_2, "value2", "Value 2"},
            {0, NULL, NULL}};

        fake_flags_type = g_flags_register_static(FLAGS_NAME, fake_values);
        g_once_init_leave(&fake_flags_id, fake_flags_type);
    }

    return fake_flags_id;
}

static const char *default_option = "0x00000005, \"Value 2+Value 0\"";
static const char *parsed_options[] = {"(0x00000001): Value 0          - value0",
                                       "(0x00000002): Value 1          - value1",
                                       "(0x00000004): Value 2          - value2",
                                       NULL};

static void check_options(const GValue *value)
{
    GArray *caps_array = g_value_get_boxed(value);

    for (gsize i = 0; i < caps_array->len; i++)
    {
        const GValue *option_value;
        const gchar *option;
        g_assert_nonnull(parsed_options[i]);

        option_value = &g_array_index(caps_array, const GValue, i);
        option = g_value_get_string(option_value);

        g_assert_cmpstr(option, ==, parsed_options[i]);
    }
}

int main(int argc, char *argv[])
{
    GParamSpec *flags_spec = NULL;
    const GValue *value = NULL;
    GstStructure *dictionary = NULL;
    gchar *type_string = NULL;

    gst_init(&argc, &argv);

    type_string = g_strdup_printf("Flags \"%s\"", FLAGS_NAME);
    flags_spec = g_param_spec_flags("test", "Test param", "Test param", GST_TYPE_FAKE_FLAGS,
                                  FAKE_VALUE_0 | FAKE_VALUE_2, G_PARAM_READWRITE);

    value = g_param_spec_get_default_value(flags_spec);
    dictionary = gst_type_reader_fill_type(flags_spec, value);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_TYPE), ==, type_string);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_VALUE, G_TYPE_STRING));
    g_assert_cmpstr(gst_structure_get_string(dictionary, KEY_VALUE), ==, default_option);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_OPTIONS, G_TYPE_ARRAY));
    check_options(gst_structure_get_value(dictionary, KEY_OPTIONS));

    g_free(type_string);
    return 0;
}
