/**
    Fake param type for param testing.
*/
#include <gst/gst.h>

#ifndef G_FAKE_PARAM_H
#define G_FAKE_PARAM_H

GType g_param_spec_fake_get_type(void);
#define G_TYPE_PARAM_FAKE (g_param_spec_fake_get_type())

GParamSpec *g_param_spec_fake(const gchar *name,
                              const gchar *nick,
                              const gchar *blurb,
                              GParamFlags flags);

typedef struct _GParamSpecFake
{
    GParamSpec pspec;
} GParamSpecFake;

/**
 * Initialize the type of GParamSpecFake
 */
GType g_param_spec_fake_get_type(void)
{
    static volatile gsize g_fake_param_type = 0;

    if (g_once_init_enter(&g_fake_param_type))
    {
        GParamSpecTypeInfo pspec_info;
        pspec_info.instance_size = sizeof(GParamSpecFake);
        pspec_info.n_preallocs = 0;
        pspec_info.instance_init = NULL;
        pspec_info.value_type = G_TYPE_INT; //Dummy
        pspec_info.finalize = NULL;
        pspec_info.value_set_default = NULL;
        pspec_info.value_validate = NULL;
        pspec_info.values_cmp = NULL;

        GType fake_param_type = g_param_type_register_static("GParamSpecFake",
                                                             &pspec_info);

        g_once_init_leave(&g_fake_param_type, fake_param_type);
    }

    return g_fake_param_type;
}

/**
 *  @brief Creates a new GParamSpecFake. For test prposes
 */
GParamSpec *g_param_spec_fake(const gchar *name,
                              const gchar *nick,
                              const gchar *blurb,
                              GParamFlags flags)
{
    return g_param_spec_internal(G_TYPE_PARAM_FAKE, name, nick, blurb, flags);
}

#endif //G_FAKE_PARAM_H
