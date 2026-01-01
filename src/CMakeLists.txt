# Add util library
add_subdirectory(util)

# Include source files
include(CMakeSourceLists.txt)

# Create executable
add_executable(grabx ${SRC_FILES})
target_link_libraries(grabx PRIVATE util_lib)

# # Create library
# add_library(grabx_lib STATIC ${SRC_FILES})

# # Link libraries
# target_link_libraries(grabx PRIVATE spdlog::spdlog)
# target_link_libraries(grabx_lib PRIVATE ${PROJECT_LIBRARIES} spdlog::spdlog)