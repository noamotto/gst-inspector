#include "param_info.h"

#define KNOWN_PARAM_FLAGS                                           \
    (G_PARAM_CONSTRUCT | G_PARAM_CONSTRUCT_ONLY |                   \
     G_PARAM_LAX_VALIDATION | G_PARAM_STATIC_STRINGS |              \
     G_PARAM_READABLE | G_PARAM_WRITABLE | GST_PARAM_CONTROLLABLE | \
     GST_PARAM_MUTABLE_PLAYING | GST_PARAM_MUTABLE_PAUSED |         \
     GST_PARAM_MUTABLE_READY)

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

    switch (G_VALUE_TYPE(&value))
    {
    case G_TYPE_STRING:
    {
        break;
    }
    case G_TYPE_BOOLEAN:
    {
        break;
    }
    case G_TYPE_ULONG:
    {
        break;
    }
    case G_TYPE_LONG:
    {
        break;
    }
    case G_TYPE_UINT:
    {
        break;
    }
    case G_TYPE_INT:
    {
        break;
    }
    case G_TYPE_UINT64:
    {
        break;
    }
    case G_TYPE_INT64:
    {
        break;
    }
    case G_TYPE_FLOAT:
    {
        break;
    }
    case G_TYPE_DOUBLE:
    {
        break;
    }
    /* fall through */
    default:
        if (G_IS_PARAM_SPEC_ENUM(m_pspec))
        {
        }
        else if (G_IS_PARAM_SPEC_FLAGS(m_pspec))
        {
        }
        else if (G_IS_PARAM_SPEC_OBJECT(m_pspec))
        {

        }
        else if (G_IS_PARAM_SPEC_BOXED(m_pspec))
        {

        }
        else if (G_IS_PARAM_SPEC_POINTER(m_pspec))
        {

        }
        else if (m_pspec->value_type == G_TYPE_VALUE_ARRAY)
        {
            GParamSpecValueArray *pvarray = G_PARAM_SPEC_VALUE_ARRAY(m_pspec);

            if (pvarray->element_spec)
            {
                n_print("%-23.23s Array of GValues of type \"%s\"", "",
                        g_type_name(pvarray->element_spec->value_type));
            }
            else
            {
                n_print("%-23.23s Array of GValues", "");
            }
        }
        else if (GST_IS_PARAM_SPEC_FRACTION(m_pspec))
        {
            GstParamSpecFraction *pfraction = GST_PARAM_SPEC_FRACTION(m_pspec);

            n_print("%-23.23s Fraction. ", "");

            g_print("Range: %d/%d - %d/%d Default: %d/%d ",
                    pfraction->min_num, pfraction->min_den,
                    pfraction->max_num, pfraction->max_den,
                    gst_value_get_fraction_numerator(&value),
                    gst_value_get_fraction_denominator(&value));
        }
        else
        {
            n_print("%-23.23s Unknown type %ld \"%s\"", "",
                    (glong)m_pspec->value_type, g_type_name(m_pspec->value_type));
        }
        break;
    }

    g_value_reset(&value);
}

void ParamInfo::get_param_keys(
    param_keys &keys) const
{
}