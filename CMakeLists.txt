cmake_minimum_required(VERSION 3.0)

include(CMakeToolsHelpers OPTIONAL)
project(gst-inspect-server)

# We'll set the module path use our modules, to find GStreamer and GLib
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(TestTools)
find_package(GStreamer REQUIRED)

# Create main target

file(GLOB_RECURSE src_files src/**/*.c src/*.c)
file(GLOB_RECURSE include_files include/**/*.h include/*.h)
add_library(gst-inspector SHARED ${src_files} ${include_files})

target_compile_definitions(gst-inspector PRIVATE "GST_INSPECTOR_EXPORT")
target_include_directories(gst-inspector PUBLIC ${GLIB2_INCLUDE_DIRS} ${GSTREAMER_INCLUDE_DIRS} "include/")
target_link_libraries(gst-inspector PUBLIC ${GLIB2_LIBRARIES} ${GSTREAMER_LIBRARIES})

if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(gst-inspector PUBLIC "-std=c99" "-pthread" )
    if (ENABLE_COVERAGE)
        target_link_libraries(gst-inspector PUBLIC gcov)
    endif (ENABLE_COVERAGE)
endif ( CMAKE_COMPILER_IS_GNUCC )

add_subdirectory(tests)
