cmake_minimum_required(VERSION 3.21)
project(C_assembler C)

set(CMAKE_C_STANDARD 90)

add_executable(C_assembler main.c files.c headers.h structs.h macro_list.c consts.h pre_assembler.c symbol_table.c data_list.c word_list.c first_pass.c entry_list.c common.c utils.c second_pass.c externals.c)
