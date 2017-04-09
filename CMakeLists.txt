cmake_minimum_required(VERSION 2.6)

include(CMakeToolsHelpers OPTIONAL)
project(gst-inspect-server)
include(CTest)

# We'll set the module path use our modules, to find GStreamer and GLib
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GStreamer REQUIRED)

file(GLOB_RECURSE src_files "src/**/*.c")
file(GLOB_RECURSE include_files "include/**/*.h")
add_library(gst-inspector SHARED ${src_files} ${include_files})

target_compile_definitions(gst-inspector PRIVATE "GST_INSPECTOR_EXPORT")
target_include_directories(gst-inspector PUBLIC ${GLIB2_INCLUDE_DIRS} ${GSTREAMER_INCLUDE_PATH} "include/")
target_link_libraries(gst-inspector PUBLIC ${GLIB2_LIBRARIES} ${GSTREAMER_LIBRARY})

add_executable(bool_param_test "tests/params/bool_param_test.c")
target_link_libraries(bool_param_test gst-inspector)
add_test(NAME bool_param_test COMMAND bool_param_test)