cmake_minimum_required(VERSION 2.6)

include(CMakeToolsHelpers OPTIONAL)
project(gst-inspect-server)
include(CTest)

# We'll set the module path use our modules, to find GStreamer and GLib
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(TestTools)
find_package(GStreamer REQUIRED)

# Configure

if (BUILD_TESTING)
    set(ENABLE_TESTING ON)
endif (BUILD_TESTING)

configure_file(config.h.in ${CMAKE_SOURCE_DIR}/include/config.h)

# Create main target

file(GLOB_RECURSE src_files src/**/*.c src/*.c)
file(GLOB_RECURSE include_files include/**/*.h include/*.h)
add_library(gst-inspector SHARED ${src_files} ${include_files})

target_compile_definitions(gst-inspector PRIVATE "GST_INSPECTOR_EXPORT")
target_include_directories(gst-inspector PUBLIC ${GLIB2_INCLUDE_DIRS} ${GSTREAMER_INCLUDE_DIRS} "include/")
target_link_libraries(gst-inspector PUBLIC ${GLIB2_LIBRARIES} ${GSTREAMER_LIBRARIES})

if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(gst-inspector PUBLIC "-std=c99" "-pthread")
endif ( CMAKE_COMPILER_IS_GNUCC )

# Testing
if (BUILD_TESTING)

target_compile_definitions(gst-inspector PRIVATE "ENABLE_TESTING")

#give the option to create code coverage report for the tests
# (currenlty supported only on Windows, using opencppcoverage)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

set(CORE_TEST_CASES
    register_element_inspector_succeeds
    register_same_element_inspector_twice_fails
    remove_exists_element_inspector_succeeds
    remove_nonexists_element_inspector_fails
    get_element_inspectors_names_works

    register_plugin_inspector_succeeds
    register_same_plugin_inspector_twice_fails
    remove_exists_plugin_inspector_succeeds
    remove_nonexists_plugin_inspector_fails
    )

#test for type reader's core functionality
set(PARAMS_CORE_TEST_CASES
    register_new_param_type_succeeds
    register_new_param_type_twice_fails
    unknown_type_calls_default_reader
    fill_type_calls_reading_function
    )

#Tests for params
set(PARAM_TEST_CASES
    bool_param_test
    fraction_param_test
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
    known_pointer_param_test
    unknown_pointer_param_test

    enum_param_test
    unknown_value_enum_param_test

    no_flags_param_test
    single_flags_param_test
    combination_flags_param_test
    )

set(BOXED_PARAM_TEST_CASES
    unknown_boxed_param_type_test

    register_boxed_content_reader_succeeds_test
    register_boxed_content_reader_twice_fails_test

    null_caps_param_type_test
    any_caps_param_type_test
    empty_caps_param_type_test
    no_features_caps_param_type_test
    sysmem_features_caps_param_type_test
    with_features_caps_param_type_test
    
    structure_param_type_test
    null_structure_param_type_test)

set(OBJECT_PARAM_TEST_CASES
    unknown_object_param_type_test
    object_param_type_test
    
    register_same_object_content_reader_twice_fails_test)

foreach(CORE_TEST_CASE ${CORE_TEST_CASES})

add_test_case("tests/core" ${CORE_TEST_CASE})
        
endforeach(CORE_TEST_CASE)

foreach(PARAMS_CORE_TEST_CASE ${PARAMS_CORE_TEST_CASES})

add_test_case("tests/params/core" ${PARAMS_CORE_TEST_CASE})
    
endforeach(PARAMS_CORE_TEST_CASE)

foreach(PARAM_TEST_CASE ${PARAM_TEST_CASES})

add_test_case("tests/params" ${PARAM_TEST_CASE})

endforeach(PARAM_TEST_CASE)

# Tests for boxed params
foreach(BOXED_PARAM_TEST_CASE ${BOXED_PARAM_TEST_CASES})

add_test_case("tests/params/boxed" ${BOXED_PARAM_TEST_CASE})

endforeach(BOXED_PARAM_TEST_CASE)

# Tests for object params
foreach(OBJECT_PARAM_TEST_CASE ${OBJECT_PARAM_TEST_CASES})

add_test_case("tests/params/object" ${OBJECT_PARAM_TEST_CASE})

endforeach(OBJECT_PARAM_TEST_CASE)

#add target to generate coverage report
if(ENABLE_COVERAGE AND MSVC)
    add_custom_target(coverage-report COMMAND opencppcoverage ${REPORT_FILES})
endif(ENABLE_COVERAGE AND MSVC)

endif (BUILD_TESTING)
