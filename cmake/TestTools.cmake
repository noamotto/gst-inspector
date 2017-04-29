##############################################################################
#                      Varity of utilities for tests                         #
##############################################################################

macro(create_test_command test_case)
    if(ENABLE_COVERAGE AND MSVC)
        set(TEST_COMMAND opencppcoverage --export_type=binary:${test_case}.cov --sources $<SHELL_PATH:${CMAKE_SOURCE_DIR}/src> -- "$<TARGET_FILE:${test_case}>")
        list(APPEND REPORT_FILES "--input_coverage=${test_case}.cov")
    else(ENABLE_COVERAGE AND MSVC)
        set(TEST_COMMAND ${test_case})
    endif(ENABLE_COVERAGE AND MSVC)
    foreach(arg IN LISTS ${ARGN})
        set(TEST_COMMAND ${TEST_COMMAND} ${arg})
    endforeach(arg)
endmacro(create_test_command)

macro(add_test_case test_prefix test_case)
    add_executable(${test_case} "${test_prefix}/${test_case}.c")
    target_link_libraries(${test_case} gst-inspector)

    create_test_command(${test_case} ${ARGN})

    add_test(NAME ${PARAM_TEST_CASE} COMMAND ${TEST_COMMAND})

    # When building on VS, add GStreamer's root to PATH when running the tests
    if(MSVC)
    set_property(TEST ${PARAM_TEST_CASE} PROPERTY ENVIRONMENT PATH=$<SHELL_PATH:${GSTREAMER_BIN_PATH}>;$ENV{Path})
    endif(MSVC)
endmacro(add_test_case)