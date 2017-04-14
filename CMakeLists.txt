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

#give the option to create code coverage report for the tests
# (currenlty supported only on Windows, using opencppcoverage)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

set(TEST_CASES 
    bool_param_test
    int_param_test
    int64_param_test
    long_param_test
    uint_param_test
    uint64_param_test
    ulong_param_test
    float_param_test
    double_param_test)

foreach(TEST_CASE ${TEST_CASES})

add_executable(${TEST_CASE} "tests/params/${TEST_CASE}.c")
target_link_libraries(${TEST_CASE} gst-inspector)

if(ENABLE_COVERAGE AND MSVC)
    set(TEST_COMMAND opencppcoverage --export_type=binary:${TEST_CASE}.cov --sources $<SHELL_PATH:${CMAKE_SOURCE_DIR}/src> -- "$<TARGET_FILE:${TEST_CASE}>")
    list(APPEND REPORT_FILES "--input_coverage=${TEST_CASE}.cov")
else(ENABLE_COVERAGE AND MSVC)
    set(TEST_COMMAND ${TEST_CASE})
endif(ENABLE_COVERAGE AND MSVC)

add_test(NAME ${TEST_CASE} COMMAND ${TEST_COMMAND})

# When building on VS, add GStreamer's root to PATH when running the tests
if(MSVC)
set_property(TEST ${TEST_CASE} PROPERTY ENVIRONMENT PATH=${GSTREAMER_BIN_PATH};$ENV{Path})
endif(MSVC)

endforeach(TEST_CASE)

#add target to generate coverage report
if(ENABLE_COVERAGE AND MSVC)
    add_custom_target(coverage-report COMMAND opencppcoverage ${REPORT_FILES})
endif(ENABLE_COVERAGE AND MSVC)

endif (BUILD_TESTING)