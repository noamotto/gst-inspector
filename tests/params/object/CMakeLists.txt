# Testing
if (BUILD_TESTING)

set(TEST_CASES
    # Core tests
    register_same_object_content_reader_twice_fails_test
    
    # General object tests
    unknown_object_param_type_test
    object_param_type_test
    null_object_param_type_test
    )

foreach(TEST_CASE ${TEST_CASES})

add_test_case(${TEST_CASE})

endforeach(TEST_CASE)

endif (BUILD_TESTING)
