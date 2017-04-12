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

if (BUILD_TESTING)

set(TEST_CASES "bool_param_test" "int_param_test" "int64_param_test" "long_param_test")

foreach(TEST_CASE ${TEST_CASES})

add_executable(${TEST_CASE} "tests/params/${TEST_CASE}.c")
target_link_libraries(${TEST_CASE} gst-inspector)
add_test(NAME ${TEST_CASE} COMMAND ${TEST_CASE})

# When building on VS, add GStreamer's root to PATH when running the tests
if(MSVC)
set_property(TEST ${TEST_CASE} PROPERTY ENVIRONMENT PATH=${GSTREAMER_BIN_PATH};$ENV{Path})
endif(MSVC)

endforeach(TEST_CASE)

endif (BUILD_TESTING)