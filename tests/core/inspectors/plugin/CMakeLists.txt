# Testing
if (BUILD_TESTING)

add_library(testplugindate SHARED testplugindate.c)
target_link_libraries(testplugindate gst-inspector)

add_library(testplugindatetime SHARED testplugindatetime.c)
target_link_libraries(testplugindatetime gst-inspector)

set(TEST_CASES
    # Plugin details inspector tests
    plugin_details_inspector_works_no_release_date
    plugin_details_inspector_works_release_date
    plugin_details_inspector_works_release_date_time
    )

foreach(TEST_CASE ${TEST_CASES})

add_test_case(${TEST_CASE})

endforeach(TEST_CASE)

set_property(TEST ${TEST_CASES} APPEND PROPERTY ENVIRONMENT GST_PLUGIN_PATH=$<TARGET_FILE_DIR:testplugindate>)

endif (BUILD_TESTING)
