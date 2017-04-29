cmake_minimum_required(VERSION 2.6)

include(CMakeToolsHelpers OPTIONAL)
project(gst-inspect-server)
include(CTest)

# We'll set the module path use our modules, to find GStreamer and GLib
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(TestTools)
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

#Tests for params
set(PARAM_TEST_CASES 
    bool_param_test
    int_param_test
    int64_param_test
    long_param_test
    uint_param_test
    uint64_param_test
    ulong_param_test
    float_param_test
    double_param_test
    string_param_test
    null_string_param_test
    )

set(BOXED_PARAM_TEST_CASES
    unknown_boxed_param_type_test)

foreach(PARAM_TEST_CASE ${PARAM_TEST_CASES})

add_test_case("tests/params" ${PARAM_TEST_CASE})

endforeach(PARAM_TEST_CASE)

# Tests for boxed params
foreach(BOXED_PARAM_TEST_CASE ${BOXED_PARAM_TEST_CASES})

add_test_case("tests/params/boxed" ${BOXED_PARAM_TEST_CASE})

endforeach(BOXED_PARAM_TEST_CASE)

#add target to generate coverage report
if(ENABLE_COVERAGE AND MSVC)
    add_custom_target(coverage-report COMMAND opencppcoverage ${REPORT_FILES})
endif(ENABLE_COVERAGE AND MSVC)

endif (BUILD_TESTING)
