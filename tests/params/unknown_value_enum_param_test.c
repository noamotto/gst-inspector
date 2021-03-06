#include "type_reader/type_reader.h"

#define ENUM_NAME ("GFakeEnum")

typedef enum _GFakeEnum {
    FAKE_VALUE_0 = 0,
    FAKE_VALUE_1 = 1,
    FAKE_VALUE_2 = 2
} GFakeEnum;

#define GST_TYPE_FAKE_ENUM (gst_fake_enum_get_type())
static GType gst_fake_enum_get_type()
{
    static volatile GType fake_enum_id = 0;

    if (g_once_init_enter(&fake_enum_id))
    {
        GType fake_enum_type;
        static const GEnumValue fake_values[] = {
            {FAKE_VALUE_0, "value0", "Value 0"},
            {FAKE_VALUE_1, "value1", "Value 1"},
            {FAKE_VALUE_2, "value2", "Value 2"},
            {0, NULL, NULL}};

        fake_enum_type = g_enum_register_static(ENUM_NAME, fake_values);
        g_once_init_leave(&fake_enum_id, fake_enum_type);
    }

    return fake_enum_id;
}

static const char *default_option = "3, \"unknown\"";
static const char *parsed_options[] = {"(0): Value 0          - value0",
                                       "(1): Value 1          - value1",
                                       "(2): Value 2          - value2",
                                       NULL};

static void check_options(const GValue *value)
{
    for (guint i = 0; i < gst_array_get_size(value); i++)
    {
        const GValue *option_value;
        const gchar *option;
        g_assert_nonnull(parsed_options[i]);

        option_value = gst_array_get_value(value, i);
        option = g_value_get_string(option_value);

        g_assert_cmpstr(option, ==, parsed_options[i]);
    }
}

int main(int argc, char *argv[])
{
    GParamSpec *enum_spec = NULL;
    GValue value = G_VALUE_INIT;
    GstStructure *dictionary;
    gchar *type_string = NULL;

    gst_init(&argc, &argv);

    dictionary = gst_structure_new_empty("test");
    type_string = g_strdup_printf("Enum \"%s\"", ENUM_NAME);
    enum_spec = g_param_spec_enum("test", "Test param", "Test param", GST_TYPE_FAKE_ENUM,
                                  FAKE_VALUE_0, G_PARAM_READWRITE);

    g_value_init(&value, GST_TYPE_FAKE_ENUM);
    g_value_set_enum(&value, 3);
    gst_type_reader_fill_type(enum_spec, &value, dictionary);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_NAME, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dictionary, KEY_NAME), ==, "test: Test param");

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_TYPE, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dictionary, KEY_TYPE), ==, type_string);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_VALUE, G_TYPE_STRING));
    g_assert_cmpstr(gst_dictionary_get_string(dictionary, KEY_VALUE), ==, default_option);

    g_assert_true(gst_structure_has_field_typed(dictionary, KEY_OPTIONS, GST_TYPE_LIST));
    check_options(gst_dictionary_get_array(dictionary, KEY_OPTIONS));

    g_free(type_string);
    gst_structure_free(dictionary);
    g_param_spec_unref(enum_spec);
    gst_deinit();

    return 0;
}
