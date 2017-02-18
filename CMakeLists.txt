cmake_minimum_required(VERSION 2.6)

project(gst-inspect-server)

# We'll set the module path use our modules, to find GStreamer and GLib
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GStreamer REQUIRED)

file(GLOB_RECURSE src_files "src/**/*.cpp")
file(GLOB_RECURSE include_files "include/**/*.h")
add_library(gst-inspector SHARED ${src_files} ${include_files})

target_include_directories(gst-inspector PRIVATE ${GLIB2_INCLUDE_DIRS} ${GSTREAMER_INCLUDE_PATH} "include/")
target_link_libraries(gst-inspector PRIVATE ${GLIB2_LIBRARIES} ${GSTREAMER_LIBRARY})