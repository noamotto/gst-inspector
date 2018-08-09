/**
    Dummy boxed type definition for boxed tests
*/

#ifndef G_FAKE_BOXED_H
#define G_FAKE_BOXED_H

#include <gst/gst.h>

#define G_TYPE_FAKE_BOXED (g_fake_boxed_get_type())

typedef struct _GFakeBoxed
{
    gint num;
}GFakeBoxed;

GFakeBoxed *g_fake_boxed_new(gint num);
GFakeBoxed *g_fake_boxed_copy(GFakeBoxed *other);
GType g_fake_boxed_get_type(void);

GFakeBoxed *g_fake_boxed_new(gint num)
{
    GFakeBoxed *new_fake = (GFakeBoxed *)g_malloc(sizeof(GFakeBoxed));

    new_fake->num = num;
    return new_fake;
}

GFakeBoxed *g_fake_boxed_copy(GFakeBoxed *other)
{
    GFakeBoxed *new_fake = (GFakeBoxed *)g_malloc(sizeof(GFakeBoxed));

    new_fake->num = other->num;
    return new_fake;
}

G_DEFINE_BOXED_TYPE(GFakeBoxed, g_fake_boxed, (GBoxedCopyFunc)g_fake_boxed_copy, g_free)

#endif //G_FAKE_BOXED_H
