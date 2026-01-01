cmake_minimum_required(VERSION 3.13)
project(grabx)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Use -std=c++17, not -std=gnu++17 (without system-specific extensions)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Important for

# Add compiler flags
if(MSVC)
    add_compile_options(/W4 /WX) # Warning level 4, treat warnings as errors
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror) # Treat warnings as errors
endif()

# OPTIONS
option(ENABLE_COVERAGE "Enable a Code Coverage build." OFF)

# Set CLang-Tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
else()
    message(STATUS "clang-tidy not found.")
endif()

# cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Force use of dynamic runtime library (MSVC)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(dependencies)
include(docs) # Doxygen documentation

if(ENABLE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()

# Enable testing (Google Test will be used)
# enable_testing()

# Collect include directories
include_directories(inc)

message(STATUS "Source dir: ${CMAKE_CURRENT_SOURCE_DIR}/src")
add_subdirectory(src)

message(STATUS "Test dir: ${CMAKE_CURRENT_SOURCE_DIR}/test")
add_subdirectory(test)
