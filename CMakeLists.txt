cmake_minimum_required(VERSION 2.6)

project(gst-inspect-server)

# We'll set the module path use our modules, to find GStreamer and GLib
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(GStreamer REQUIRED)

include_directories(${GLIB2_INCLUDE_DIRS})