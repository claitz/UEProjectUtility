cmake_minimum_required(VERSION 3.16)
project(renameProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(renameProject main.cpp)

find_path(TERMCOLOR_INCLUDE_DIRS termcolor/termcolor.hpp)

if(NOT TERMCOLOR_INCLUDE_DIRS)
    message(FATAL_ERROR "Termcolor not found!")
endif()

target_include_directories(renameProject PRIVATE ${TERMCOLOR_INCLUDE_DIRS})