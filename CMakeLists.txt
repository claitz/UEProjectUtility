cmake_minimum_required(VERSION 3.16)
project(UEProjectUtility)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Executable
add_executable(UEProjectUtility ${SOURCES})

# Termcolor library
find_path(TERMCOLOR_INCLUDE_DIRS termcolor/termcolor.hpp)
if(NOT TERMCOLOR_INCLUDE_DIRS)
    message(FATAL_ERROR "Termcolor not found!")
endif()

target_include_directories(UEProjectUtility PRIVATE ${TERMCOLOR_INCLUDE_DIRS})
