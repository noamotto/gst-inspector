# Testing
if (BUILD_TESTING)

set(TEST_CASES
    # Element inspector API tests
    register_element_inspector_succeeds
    register_same_element_inspector_twice_fails
    remove_exists_element_inspector_succeeds
    remove_nonexists_element_inspector_fails
    get_element_inspectors_names_works
    element_inspector_works
    element_inspector_invalid_element_fails

    # Plugin inspector API tests
    register_plugin_inspector_succeeds
    register_same_plugin_inspector_twice_fails
    remove_exists_plugin_inspector_succeeds
    remove_nonexists_plugin_inspector_fails
    get_plugin_inspectors_names_works
    plugin_inspector_works
    pluign_inspector_invalid_plugin_fails

    # Plugin feature inspection tests
    invalid_plugin_feature_inspector_fails
    element_plugin_feature_inspector_works
    typefind_plugin_feature_inspector_works
    tracer_plugin_feature_inspector_works

    # By name inspection tests
    element_inspect_by_name_works
    plugin_inspect_by_name_works
    typefind_inspect_by_name_works
    tracer_inspect_by_name_works
    null_inspect_by_name_fails
    nonexists_feature_inspect_by_name_fails
    element_preceeds_plugin_by_name_inspection
    )

foreach(TEST_CASE ${TEST_CASES})

add_test_case(${TEST_CASE})

endforeach(TEST_CASE)

add_subdirectory(inspectors)

endif (BUILD_TESTING)
