//
// Created by niki on 5/6/26.
//

#include "help_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 100

void remove_newline(char *str){
    str[strcspn(str, "\n")] = '\0';
}

void clear_stdin() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

bool find_field_in_file(FILE *file, const char *field) {
    char field_name[MAX_STRING_SIZE];

    while (fscanf(file, " %99s", field_name) == 1) {
        if (strcmp(field_name, field) == 0) {
            skip_whitespaces(file);         //to skip the spaces between field name and value("Field:  ...  value")
            return true;
        }
    }

    printf("Couldn't find field \"%s\" in file!\n", field);
    return false;
}

void skip_whitespaces(FILE * file) {
    int c;
    while ((c = fgetc(file)) == ' ' || c == '\t' || c == '\n') {
        // skip whitespaces
    }
    if (c != EOF) {
        ungetc(c, file);
    }
}

int int_string_length(const int value) {
    //sprintf - returns the number of characters that would have been written, not counting the terminating null character.
    return snprintf(NULL, 0, "%d", value);
}