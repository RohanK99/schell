cmake_minimum_required(VERSION 3.8)

project(schell LANGUAGES C)

add_executable(schell main.c include/builtin.h include/command.h)
target_include_directories(schell PUBLIC include)

if (CMAKE_BUILD_TYPE EQUAL Debug)
    target_compile_options(schell PRIVATE -Wall -Wextra)
endif()

