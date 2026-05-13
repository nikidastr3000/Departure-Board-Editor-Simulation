#include <stdio.h>
#include <stdlib.h>

#include "departure_board_editor.h"


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

    char binary[MAX_STRING_SIZE] = "sprites_binary.txt";
    char text[MAX_STRING_SIZE] = "sprites.txt";

    Sprite **sprites = input_sprites_from_file(text);
    output_sprites(sprites, stdout);

    FILE *fp = fopen(binary, "wb");
    output_sprites_binary(sprites, fp);
    fclose(fp);

    Sprite **sprites_bin = input_sprites_binary(binary);

    output_sprites(sprites_bin, stdout);

    delete_sprites_array(sprites);
    delete_sprites_array(sprites_bin);
    return 0;
}