cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_STANDARD 90)

project(assembler C)

set (CMAKE_CXX_FLAGS "-ansi -pedantic -Wall")
add_executable(assembler main.c)
