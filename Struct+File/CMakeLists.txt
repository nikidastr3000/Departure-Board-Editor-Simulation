cmake_minimum_required(VERSION 4.1)
project(Struct_File C)

set(CMAKE_C_STANDARD 23)

add_executable(Struct_File main.c
        db_structures.h
        functions/src/help_functions.c
        functions/include/help_functions.h
        departure_board_editor.h
        functions/src/input_output.c
        functions/include/input_output.h
        functions/src/one_sprite.c
        functions/include/one_sprite.h
        functions/src/many_sprites.c
        functions/include/many_sprites.h)

include_directories(functions/include .)        #unsure about this