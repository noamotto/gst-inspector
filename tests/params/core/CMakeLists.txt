# Testing
if (BUILD_TESTING)

set(TEST_CASES
    register_new_param_type_succeeds
    register_new_param_type_twice_fails
    unknown_type_calls_default_reader
    fill_type_calls_reading_function
    )

foreach(TEST_CASE ${TEST_CASES})

add_test_case(${TEST_CASE})

endforeach(TEST_CASE)

endif (BUILD_TESTING)
