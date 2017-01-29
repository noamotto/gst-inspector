#pragma once

#include <gst/gst.h>
#include <map>

//Common types and utility functions
typedef std::map<const char*, char*> param_keys;

gchar *concat_on_the_fly(
    gchar *str1,
    const gchar *str2);

//Param keys
#define KEY_NAME        ("Name")
#define KEY_FLAGS       ("Flags")
#define KEY_TYPE        ("Type")
#define KEY_VALUE       ("Default Value")
#define KEY_RANGE       ("Range")
#define KEY_OPTIONS     ("Options")
#define KEY_CONTENT     ("Content")