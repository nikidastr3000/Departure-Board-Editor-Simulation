#include <stdio.h>
#include <stdlib.h>

#include "departure_board_editor.h"

void test_input_output();

int main(void) {
    ScreenType screen = {.width = 60, .height = 15, .bg_char = 1};

    fill_screen(&screen);

    const char spriteInputFile[MAX_STRING_SIZE] = "sprites.txt";
    /*FILE *fp = fopen(spriteInputFile, "r+");

    Sprite spr;
    input_sprite(&spr);
    output_sprite(&spr, fp);

    fclose(fp);*/

    Sprite **sprites = input_sprites_from_file(spriteInputFile);
    //output_sprites(sprites, stdout);

    ScreenType test_screen = {.width = 60, .height = 15, .bg_char = 1};
    fill_screen(&test_screen);
    check_sprites((const Sprite **)sprites, &test_screen);
    printf("Test screen: \n");
    output_screen(test_screen, TODO);
    delete_screen(&test_screen);

    display_sprites((const Sprite **)sprites, &screen);
    output_screen(screen, TODO);

    delete_sprites_array(sprites);
    delete_screen(&screen);

    return 0;
}

void test_input_output(){
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
}