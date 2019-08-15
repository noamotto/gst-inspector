# Testing
if (BUILD_TESTING)

add_executable(simple_inspection simple_inspection.c)
target_link_libraries(simple_inspection gst-inspector)

add_executable(simple_listing simple_listing.c)
target_link_libraries(simple_listing gst-inspector)

add_executable(gst_inspect gst_inspect.c)
target_link_libraries(gst_inspect gst-inspector)

endif (BUILD_TESTING)
