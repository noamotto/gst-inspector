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
    element_interface_inspector_works

    #Element pad templates inspector tests
    element_pad_templates_inspector_no_templs
    element_pad_templates_inspector_always
    element_pad_templates_inspector_sometimes
    element_pad_templates_inspector_request
    element_pad_templates_inspector_props

    #Element clocking inspector tests
    element_clocking_inspector_no_clocking
    element_clocking_inspector_requires
    element_clocking_inspector_requires_and_provides
    element_clocking_inspector_requires_and_provides_null

    #Element URI capabilities tests
    element_uri_inspector_no_uri
    element_uri_inspector_src_no_protocols
    element_uri_inspector_src_with_protocols
    element_uri_inspector_sink_no_protocols
    element_uri_inspector_sink_with_protocols
    element_uri_inspector_unknown_no_protocols
    element_uri_inspector_unknown_with_protocols
    )

foreach(TEST_CASE ${TEST_CASES})

add_test_case(${TEST_CASE})

endforeach(TEST_CASE)

endif (BUILD_TESTING)
