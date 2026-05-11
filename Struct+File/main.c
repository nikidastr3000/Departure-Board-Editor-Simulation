#include <stdio.h>
#include <stdlib.h>

#include "db_structures.h"
#include "db_functions.h"


int main(void) {
    const int screen_width = 80;
    const int screen_height = 40;
    ScreenBuffer screen = {.width = screen_width, .height = screen_height};

    screen.buffer = (char **) malloc(sizeof(char *) * screen_height);
    for (int i = 0; i < screen_height; i++) {
        screen.buffer[i] = (char *) malloc(sizeof(char) * (screen_width + 1));
        for (int j = 0; j < screen_width; j++) {
            screen.buffer[i][j] = ' ';
        }
        screen.buffer[i][screen_width] = '\0';
    }

    char filename[MAX_STRING_SIZE] = "sprites.txt";
    Sprite **sprites = input_sprites_from_file(filename);
    output_sprites(sprites, stdout);


}