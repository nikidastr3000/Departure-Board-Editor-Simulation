cmake_minimum_required(VERSION 4.1)
project(Struct_File C)

set(CMAKE_C_STANDARD 23)

add_executable(Struct_File main.c
        db_structures.h
        db_functions.c
        db_functions.h
        help_functions.c
        help_functions.h)
