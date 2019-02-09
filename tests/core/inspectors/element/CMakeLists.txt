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

    #Element URI capabilities inspector tests
    element_uri_inspector_no_uri
    element_uri_inspector_src_no_protocols
    element_uri_inspector_src_with_protocols
    element_uri_inspector_sink_no_protocols
    element_uri_inspector_sink_with_protocols
    element_uri_inspector_unknown_no_protocols
    element_uri_inspector_unknown_with_protocols

    #Element pads inspector tests
    element_pads_inspector_no_pads
    element_pads_inspector_pads_no_caps
    element_pads_inspector_pads_caps_no_template
    element_pads_inspector_pads_caps_template

    #Element signals inspctor tests
    element_signals_inspector_no_signals
    element_signals_inspector_only_actions
    element_signals_inspector_no_args
    element_signals_inspector_args
    element_signals_inspector_sometimes_pad
    
    #Element actions inspctor tests
    element_actions_inspector_no_actions
    element_actions_inspector_only_signals
    element_actions_inspector_no_args
    element_actions_inspector_args

    #Element children inspctor tests
    element_children_inspector_no_bin
    element_children_inspector_no_children
    element_children_inspector_works

    #Element preset inspctor tests
    element_preset_inspector_not_preset
    element_preset_inspector_null_preset
    element_preset_inspector_empty_list
    element_preset_inspector_full_list
    )

foreach(TEST_CASE ${TEST_CASES})

add_test_case(${TEST_CASE})

endforeach(TEST_CASE)

endif (BUILD_TESTING)
