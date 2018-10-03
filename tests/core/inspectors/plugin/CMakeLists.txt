# Testing
if (BUILD_TESTING)

set(TEST_CASES
    # Plugin details inspector tests
    plugin_details_inspector_works_no_release_date
    )

foreach(TEST_CASE ${TEST_CASES})

add_test_case(${TEST_CASE})

endforeach(TEST_CASE)

endif (BUILD_TESTING)
