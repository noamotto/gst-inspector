# Look for GStreamer
if(MSVC)
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(GLIB2_ROOT $ENV{GSTREAMER_1_0_ROOT_X86_64})
    else("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(GLIB2_ROOT $ENV{GSTREAMER_1_0_ROOT_X86})    
    endif("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")    
endif(MSVC)
find_package(GLib2 REQUIRED)

message("Looking for GStreamer")

if(MSVC)
    set(GSTREAMER_ROOT_PATH $ENV{GSTREAMER_1_0_ROOT_X86})
    if(NOT <STREQUAL:${GSTREAMER_ROOT_PATH},"">)
        message(STATUS "Found root at " ${GSTREAMER_ROOT_PATH})
        
        find_library(GSTREAMER_LIBRARY gstreamer-1.0
                  HINTS ${GSTREAMER_ROOT_PATH}
                  PATH_SUFFIXES "lib/")

        find_path(GSTREAMER_BIN_PATH libgstreamer-1.0-0.dll
                  HINTS ${GSTREAMER_ROOT_PATH}
                  PATH_SUFFIXES "bin/")
        
        find_path(GSTREAMER_INCLUDE_PATH gst/gst.h
                  HINTS ${GSTREAMER_ROOT_PATH}
                  PATH_SUFFIXES "include/gstreamer-1.0/")

    else(NOT <STREQUAL:${GSTREAMER_ROOT_PATH},"">)
        message(SEND_ERROR "Could not find GStreamer's root path")
    endif(NOT <STREQUAL:${GSTREAMER_ROOT_PATH},"">)
else(MSVC)
    include(FindPkgConfig)

	if ( PKG_CONFIG_FOUND )
		PKG_SEARCH_MODULE( GSTREAMER REQUIRED gstreamer-1.0 )
	endif ( PKG_CONFIG_FOUND )
endif(MSVC)