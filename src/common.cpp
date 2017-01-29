#include "include/common.h"

gchar *concat_on_the_fly(
    gchar *str1,
    const gchar *str2)
{
    gchar *result = g_strconcat(str1, str2);
    g_free(str1);
    return result;
}