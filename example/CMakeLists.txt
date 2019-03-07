# Testing
if (BUILD_TESTING)

add_executable(simple_inspection simple_inspection.c)
target_link_libraries(simple_inspection gst-inspector)

endif (BUILD_TESTING)
