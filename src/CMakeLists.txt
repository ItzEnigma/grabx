# Get source files
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Create executable
add_executable(grabx ${SRC_FILES})

# Create library
add_library(grabx_lib STATIC ${SRC_FILES})

# Link libraries
target_link_libraries(grabx_lib PRIVATE ${PROJECT_LIBRARIES})