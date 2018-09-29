# Testing
if (BUILD_TESTING)

add_subdirectory("core")
add_subdirectory("params")

#add target to generate coverage report
if(ENABLE_COVERAGE)
    if (MSVC)
        string(REPLACE " " "\n" OPENCPPCOVERAGE_CONF ${REPORT_FILES})
        configure_file(${PROJECT_SOURCE_DIR}/opencppcoverage_config.txt.in ${PROJECT_SOURCE_DIR}/opencppcoverage_config.txt)
        add_custom_target(coverage-report COMMAND cd $<SHELL_PATH:$<TARGET_FILE_DIR:gst-inspector>> && opencppcoverage --config_file $<SHELL_PATH:${PROJECT_SOURCE_DIR}/opencppcoverage_config.txt>)
    else(MSVC)
    endif(MSVC)
endif(ENABLE_COVERAGE)

endif (BUILD_TESTING)
