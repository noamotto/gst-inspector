# Testing
if (BUILD_TESTING)

set(TEST_CASES
    # Element property inspector tests
    param_inspector_works
    param_inspector_nonreadable_param_works
    )

foreach(TEST_CASE ${TEST_CASES})

add_test_case(${TEST_CASE})

endforeach(TEST_CASE)

endif (BUILD_TESTING)
