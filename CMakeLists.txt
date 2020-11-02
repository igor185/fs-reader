cmake_minimum_required(VERSION 3.15)
project(reader
        VERSION 1.0.0
        DESCRIPTION "Add two numbers"
        HOMEPAGE_URL "https://github.com/chernetskyi/cpp-template"
        LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Options
option(WARNINGS_AS_ERRORS   "Treat compiler warnings as errors."    ON)
option(ENABLE_CONAN         "Use Conan as a package manager."       OFF)
option(ENABLE_PVS_STUDIO    "Use PVS-Studio static code analyzer."  OFF) # Option for the local usage only. PVS-Studio isn't installed on GitHub machines.
option(ENABLE_SANITIZERS    "Use sanitizers to detect errors."      OFF) # Option for the test builds. Do not use it in the production builds.

# Project source compilation
include_directories(inc)
add_library(fat16 STATIC
            src/fat16.cpp inc/FAT16.h src/fat16.cpp)
add_executable(reader src/main.cpp)
target_link_libraries(reader fat16)


# Define ALL_TARGETS variable to use in some directives
set(ALL_TARGETS fat16 reader)

# Include default CMake configuration
include(cmake/Config.cmake)
