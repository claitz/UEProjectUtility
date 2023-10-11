cmake_minimum_required(VERSION 3.16)
project(UEProjectUtility)

set(CMAKE_CXX_STANDARD 17)

# Use vcpkg toolchain if available
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Executable
add_executable(UEProjectUtility ${SOURCES})

# Termcolor library
find_path(TERMCOLOR_INCLUDE_DIRS "termcolor/termcolor.hpp")
target_include_directories(UEProjectUtility PRIVATE ${TERMCOLOR_INCLUDE_DIRS})
