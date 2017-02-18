#include "param_info.h"

#include "type_reader/type_resolver/type_resolver.cpp"

#define KNOWN_PARAM_FLAGS                                           \
    (G_PARAM_CONSTRUCT | G_PARAM_CONSTRUCT_ONLY |                   \
     G_PARAM_LAX_VALIDATION | G_PARAM_STATIC_STRINGS |              \
     G_PARAM_READABLE | G_PARAM_WRITABLE | GST_PARAM_CONTROLLABLE | \
     GST_PARAM_MUTABLE_PLAYING | GST_PARAM_MUTABLE_PAUSED |         \
     GST_PARAM_MUTABLE_READY)

gchar *concat_on_the_fly(
    gchar *str1,
    const gchar *str2)
{
    gchar *result = g_strconcat(str1, str2);
    g_free(str1);
    return result;
}

void ParamInfo::read_param_flags()
{
    gboolean first_flag = TRUE;
    gboolean readable = FALSE;
    gchar *flags;

    if (m_pspec->flags & G_PARAM_READABLE)
    {
        readable = TRUE;
        flags = g_strdup("readable");
        first_flag = FALSE;
    }

    if (m_pspec->flags & G_PARAM_WRITABLE)
    {
        if (first_flag)
        {
            flags = g_strdup("readable");
            first_flag = FALSE;
        }
        else
        {
            flags = concat_on_the_fly(flags, ", writable");
        }
    }
    if (m_pspec->flags & GST_PARAM_CONTROLLABLE)
    {
        flags = concat_on_the_fly(flags, ", controllable");
        first_flag = FALSE;
    }
    if (m_pspec->flags & GST_PARAM_MUTABLE_PLAYING)
    {
        flags = concat_on_the_fly(flags, ", changeable in NULL, READY, PAUSED or PLAYING state");
    }
    else if (m_pspec->flags & GST_PARAM_MUTABLE_PAUSED)
    {
        flags = concat_on_the_fly(flags, ", changeable only in NULL, READY or PAUSED state");
    }
    else if (m_pspec->flags & GST_PARAM_MUTABLE_READY)
    {
        flags = concat_on_the_fly(flags, ", changeable only in NULL or READY state");
    }
    if (m_pspec->flags & ~KNOWN_PARAM_FLAGS)
    {
        gchar *other_flags = g_strdup_printf("%s0x%0x", (first_flag) ? "" : ", ",
                                             m_pspec->flags & ~KNOWN_PARAM_FLAGS);

        flags = concat_on_the_fly(flags, other_flags);
        g_free(other_flags);
    }

    m_param_keys[KEY_FLAGS] = flags;
}

ParamInfo::ParamInfo(
    GParamSpec *const pspec,
    GstElement *element) : m_pspec(pspec)
{
    TypeResolver resolver;
    GValue value = G_VALUE_INIT;
    g_value_init(&value, m_pspec->value_type);

    m_param_keys[KEY_NAME] = g_strdup_printf("%s: %s\n", m_pspec->name,
                                             g_param_spec_get_blurb(m_pspec));

    read_param_flags();

    if (m_pspec->flags & G_PARAM_READABLE)
    {
        g_object_get_property(G_OBJECT(element), m_pspec->name, &value);
    }
    else
    {
        g_param_value_set_default(m_pspec, &value);
    }

    const ITypeReader *reader = resolver.resolve_type(m_pspec);

    if (NULL != reader)
    {
        reader->fill_type(m_pspec, &value, m_param_keys);
    }
    else
    {
        m_param_keys[KEY_TYPE] = g_strdup_printf("Unknown type %ld \"%s\"", "",
                                                 (glong)m_pspec->value_type, g_type_name(m_pspec->value_type));
    }

    g_value_reset(&value);
}

void ParamInfo::get_param_keys(
    param_keys &keys) const
{
    keys = m_param_keys;
}