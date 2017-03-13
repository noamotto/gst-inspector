#ifndef COMMON_H
#define COMMON_H

#include <gst/gst.h>

#ifdef _WIN32
    #ifdef GST_INSPECTOR_EXPORT
        #define GST_INSPECTOR_API __declspec(dllexport)
    #else
        #define GST_INSPECTOR_API __declspec(dllimport)
    #endif
#else
    #define GST_INSPECTOR_API 
#endif //_WIN32

//Param keys
#define KEY_NAME        ("Name")
#define KEY_FLAGS       ("Flags")
#define KEY_TYPE        ("Type")
#define KEY_VALUE       ("Default Value")
#define KEY_RANGE       ("Range")
#define KEY_OPTIONS     ("Options")
#define KEY_CONTENT     ("Content")

#endif //COMMON_H