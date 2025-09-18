cmake_minimum_required(VERSION 3.13)
project(grabx)

# Collect all header files
include_directories(inc)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags
add_compile_options(-Wall -Wextra -Wpedantic -Werror) # Treat warnings as errors

# Set CLang-Tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
else()
    message(STATUS "clang-tidy not found.")
endif()

# Find dependencies (libraries)
find_package(Boost REQUIRED COMPONENTS json)
include_directories(${Boost_INCLUDE_DIRS})

# Create a variable to hold all project libraries
set(PROJECT_LIBRARIES ${Boost_LIBRARIES})

# Enable testing (Google Test will be used)
enable_testing()

message(STATUS "Source dir: ${CMAKE_CURRENT_SOURCE_DIR}/src")
add_subdirectory(src)

message(STATUS "Test dir: ${CMAKE_CURRENT_SOURCE_DIR}/test")
add_subdirectory(test)
