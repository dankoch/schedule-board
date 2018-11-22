# SCHEDULE BOARD LED 
cmake_minimum_required(VERSION 3.11.0)
project(scheduleBoard)

# Set product version.
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# Create and set the output directories.
set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

# Add safeguard checks.
include(cmake/safeguards.cmake)

# Set compiler flags.
set (CMAKE_CXX_STANDARD 11)
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# Set ourselves for C99.
if(CMAKE_COMPILER_IS_GNUCC OR
   CMAKE_COMPILER_IS_GNUCXX OR
   ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
  set(CMAKE_C_FLAGS "-std=c99")
endif()

# Add configuration header.
configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/config.h.in
    ${PROJECT_BINARY_DIR}/config.h
)

# Create our core project library.
add_library(scheduleBoardLib STATIC)

# Create our executables and link them against the core library.
add_executable(scheduleBoard src/main.cpp)
target_link_libraries(scheduleBoard scheduleBoardLib)

add_executable(scheduleBoardTest)
target_link_libraries(scheduleBoardTest scheduleBoardLib)

# Include our own headers.
include_directories(include)

# Import the Source Code and initate the executable.
include(src/CMakeLists.txt)

# Import Depedency CMake file.
include(lib/CMakeLists.txt)

# Import Test Setup File
include(test/CMakeLists.txt)