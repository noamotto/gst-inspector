# Testing
if (BUILD_TESTING)

add_subdirectory("core")
add_subdirectory("params")

#add target to generate coverage report
if(ENABLE_COVERAGE)
    if (MSVC)
        add_custom_target(coverage-report COMMAND cd $<SHELL_PATH:$<TARGET_FILE_DIR:gst-inspector>> && opencppcoverage ${REPORT_FILES})
    else(MSVC)
    endif(MSVC)
endif(ENABLE_COVERAGE)

endif (BUILD_TESTING)
