//
// Created by niki on 5/6/26.
//

#ifndef STRUCT_FILE_HELP_FUNCTIONS_H
#define STRUCT_FILE_HELP_FUNCTIONS_H
#include <stdio.h>

void remove_newline(char *str);

void clear_stdin();

// moves the cursor in the file to the beginning of the field value
void find_field_in_file(FILE *file, const char *field);

void skip_whitespaces(FILE *file);

int int_string_length(int value);

#endif //STRUCT_FILE_HELP_FUNCTIONS_H