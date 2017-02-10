#include "type_reader/pointer_reader.h"

void PointerTypeReader::fill_type(
    GParamSpec *const pspec,
    GValue *const,
    param_keys &keys)
{
    if (pspec->value_type != G_TYPE_POINTER)
    {
        keys[KEY_TYPE] = g_strdup_printf("Pointer of type \"%s\".",
                g_type_name(pspec->value_type));
    }
    else
    {
        keys[KEY_TYPE] = g_strdup("Pointer.");
    }
}