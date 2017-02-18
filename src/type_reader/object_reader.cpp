#include "type_reader/object_reader.h"

void ObjectTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const ,
    param_keys &keys) const
{
    keys[KEY_TYPE] = g_strdup_printf("Object of type \"%s\"",
                                     g_type_name(pspec->value_type));
}