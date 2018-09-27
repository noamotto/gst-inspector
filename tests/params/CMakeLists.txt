# Testing
if (BUILD_TESTING)

set(TEST_CASES
    # Primitive test cases
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

    # String test cases
    string_param_test
    null_string_param_test

    # Pointer test cases
    known_pointer_param_test
    unknown_pointer_param_test

    # Value array test cases
    known_value_array_param_test
    unknown_value_array_param_test

    # Enum test cases
    enum_param_test
    unknown_value_enum_param_test

    # Flags test cases
    no_flags_param_test
    single_flags_param_test
    combination_flags_param_test
    )

foreach(TEST_CASE ${TEST_CASES})

add_test_case(${TEST_CASE})

endforeach(TEST_CASE)

add_subdirectory("core")
add_subdirectory("boxed")
add_subdirectory("object")

endif (BUILD_TESTING)
