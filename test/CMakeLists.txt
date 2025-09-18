# Get test source files
file(GLOB TEST_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create test executable
add_executable(grabx_tests ${TEST_FILES})

# Link test executable with GoogleTest and project library
target_link_libraries(grabx_tests PRIVATE grabx_lib gtest_main gmock_main)

# Enable CTest
include(GoogleTest)
gtest_discover_tests(grabx_tests)