/**
    Dummy object type definition for object tests
*/

#ifndef G_FAKE_OBJECT_H
#define G_FAKE_OBJECT_H

#include <gst/gst.h>

GType g_fake_object_get_type();
#define G_TYPE_FAKE_OBJECT (g_fake_object_get_type())

typedef struct _GFakeObject
{
    GObject object;

    gint num;
}GFakeObject;

typedef struct _GFakeObjectClass
{
    GObjectClass klass;
}GFakeObjectClass;

G_DEFINE_TYPE(GFakeObject, g_fake_object, G_TYPE_OBJECT)

void g_fake_object_class_init(GFakeObjectClass *klass)
{
    //does nothing
}

void g_fake_object_init(GFakeObject *self)
{
    self->num = 0;
}

#endif //G_FAKE_OBJECT_H
