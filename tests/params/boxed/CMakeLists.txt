# Testing
if (BUILD_TESTING)

set(TEST_CASES
    # Core tests
    register_boxed_content_reader_succeeds_test
    register_boxed_content_reader_twice_fails_test

    # Unknown boxed test
    unknown_boxed_param_type_test

    # GstCaps tests
    null_caps_param_type_test
    any_caps_param_type_test
    empty_caps_param_type_test
    no_features_caps_param_type_test
    sysmem_features_caps_param_type_test
    with_features_caps_param_type_test
    
    # GstStructure tests
    structure_param_type_test
    null_structure_param_type_test)

foreach(TEST_CASE ${TEST_CASES})

add_test_case(${TEST_CASE})

endforeach(TEST_CASE)

endif (BUILD_TESTING)
