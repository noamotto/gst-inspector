# GLib2 finding script
# Will define:
# GLIB2_FOUND
# GLIB2_INCLUDE_DIRS
# GLIB2_LIBRARIES


message("Looking for GLib-2")

if (GLIB2_LIBRARIES AND GLIB2_INCLUDE_DIRS )

  # in cache already
  set(GLIB2_FOUND TRUE)
else (GLIB2_LIBRARIES AND GLIB2_INCLUDE_DIRS )
  
  if (MSVC)
  	#Look for Glib2's dependencies via the given GLIB2_ROOT, as pkg-config should not work
	find_path(_glibconfig_include_DIR
    NAMES glibconfig.h
    PATHS ${GLIB2_ROOT}/lib
    PATH_SUFFIXES glib-2.0/include
    )

    find_path(_glib2_include_DIR
    NAMES glib.h
    PATHS ${GLIB2_ROOT}/include
    PATH_SUFFIXES glib-2.0
    )

	find_library(_glib2_link_DIR
    NAMES glib-2.0 glib
    PATHS ${GLIB2_ROOT}/lib
    )

	if ( _glib2_include_DIR AND _glib2_link_DIR )
        set ( GLIB2_INCLUDE_DIRS ${_glib2_include_DIR} ${_glibconfig_include_DIR} )
        set ( GLIB2_LIBRARIES ${_glib2_link_DIR} )
        set ( GLIB2_CORE_FOUND TRUE )
    else ( _glib2_include_DIR AND _glib2_link_DIR )
        set ( GLIB2_CORE_FOUND FALSE )
  endif ( _glib2_include_DIR AND _glib2_link_DIR )

  else(MSVC)
    #Try to find via PkgConfig
    include(FindPkgConfig)

	if ( PKG_CONFIG_FOUND )
		PKG_SEARCH_MODULE( GLIB2 REQUIRED glib-2.0 )
		if ( GLIB2_FOUND )
			set ( GLIB2_CORE_FOUND TRUE )
		else ( GLIB2_FOUND )
			set ( GLIB2_CORE_FOUND FALSE )
		endif ( GLIB2_FOUND )
	endif ( PKG_CONFIG_FOUND )
  endif(MSVC)
    
    # Handle dependencies
    # libintl
    if ( NOT LIBINTL_FOUND )
      find_path(LIBINTL_INCLUDE_DIR
      NAMES
        libintl.h
      PATHS
        ${GLIB2_ROOT}/include
        /opt/gnome/include
        /opt/local/include
        /sw/include
        /usr/include
        /usr/local/include
      )

      FIND_LIBRARY(LIBINTL_LIBRARY
      NAMES
        intl
      PATHS
        ${GLIB2_ROOT}/lib
        /opt/gnome/lib
        /opt/local/lib
        /sw/lib
        /usr/local/lib
        /usr/lib
      )

      if (LIBINTL_LIBRARY AND LIBINTL_INCLUDE_DIR)
        set (LIBINTL_FOUND TRUE)
      endif (LIBINTL_LIBRARY AND LIBINTL_INCLUDE_DIR)
    endif ( NOT LIBINTL_FOUND )

    # libiconv
    if ( NOT LIBICONV_FOUND )
      find_path(LIBICONV_INCLUDE_DIR
      NAMES
        iconv.h
      PATHS
        ${GLIB2_ROOT}/include
        /opt/gnome/include
        /opt/local/include
        /opt/local/include
        /sw/include
        /sw/include
        /usr/local/include
        /usr/include
      PATH_SUFFIXES
        glib-2.0
      )

      find_library(LIBICONV_LIBRARY
      NAMES
        iconv
      PATHS
        ${GLIB2_ROOT}/lib
        /opt/gnome/lib
        /opt/local/lib
        /sw/lib
        /usr/lib
        /usr/local/lib
      )

      if (LIBICONV_LIBRARY AND LIBICONV_INCLUDE_DIR)
        set (LIBICONV_FOUND TRUE)
      endif (LIBICONV_LIBRARY AND LIBICONV_INCLUDE_DIR)
    endif ( NOT LIBICONV_FOUND )

    if (LIBINTL_FOUND)
      set (GLIB2_LIBRARIES ${GLIB2_LIBRARIES} ${LIBINTL_LIBRARY})
      set (GLIB2_INCLUDE_DIRS ${GLIB2_INCLUDE_DIRS} ${LIBINTL_INCLUDE_DIR})
    endif (LIBINTL_FOUND)

    if (LIBICONV_FOUND)
      set (GLIB2_LIBRARIES ${GLIB2_LIBRARIES} ${LIBICONV_LIBRARY})
      set (GLIB2_INCLUDE_DIRS ${GLIB2_INCLUDE_DIRS} ${LIBICONV_INCLUDE_DIR})
    endif (LIBICONV_FOUND)
  ##

  if (GLIB2_CORE_FOUND AND GLIB2_INCLUDE_DIRS AND GLIB2_LIBRARIES)
    set (GLIB2_FOUND TRUE)
  endif (GLIB2_CORE_FOUND AND GLIB2_INCLUDE_DIRS AND GLIB2_LIBRARIES)

  if (GLIB2_FOUND)
    if (NOT GLIB2_FIND_QUIETLY)
      message (STATUS "Found GLib2: ${GLIB2_LIBRARIES} ${GLIB2_INCLUDE_DIRS}")
    endif (NOT GLIB2_FIND_QUIETLY)
  else (GLIB2_FOUND)
    if (GLIB2_FIND_REQUIRED)
      message (SEND_ERROR "Could not find GLib2")
    endif (GLIB2_FIND_REQUIRED)
  endif (GLIB2_FOUND)

  # show the GLIB2_INCLUDE_DIRS and GLIB2_LIBRARIES variables only in the advanced view
  MARK_AS_ADVANCED(GLIB2_INCLUDE_DIRS GLIB2_LIBRARIES)
  MARK_AS_ADVANCED(LIBICONV_INCLUDE_DIR LIBICONV_LIBRARY)
  MARK_AS_ADVANCED(LIBINTL_INCLUDE_DIR LIBINTL_LIBRARY)

endif (GLIB2_LIBRARIES AND GLIB2_INCLUDE_DIRS)
