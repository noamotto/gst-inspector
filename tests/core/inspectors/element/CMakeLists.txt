# Testing
if (BUILD_TESTING)

set(TEST_CASES
    # Element property inspector tests
    param_inspector_works
    param_inspector_nonreadable_param_works

    #Element factory inspector tests
    element_factory_inspector_works
    element_factory_inspector_works_additional_metadata

    #Element plugin inspector tests
    element_plugin_inspector_works
    static_element_plugin_inspector_does_nothing

    #Element hierarchy inspector test
    element_hierarchy_inspector_works

    #Element interfaces inspector tests
    element_interface_inspector_no_ifaces_does_nothing
    )

foreach(TEST_CASE ${TEST_CASES})

add_test_case(${TEST_CASE})

endforeach(TEST_CASE)

endif (BUILD_TESTING)
