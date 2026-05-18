//
// Created by niki on 5/13/26.
//

#include "many_sprites.h"

#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "input_output.h"

void add_sprite_to_sprites(const Sprite *sprite) {
    int size = 0;
    for (; SPRITES[size] != NULL; size++) {}

    Sprite **temp = realloc(SPRITES, size + 2);     //sprites[size+1] is the last sprite
    if (temp == NULL) {
        perror("realloc");
        FILE *fp = fopen("last_usage.txt", "w");
        output_sprites(SPRITES, fp);
        fclose(fp);
        exit(1);
    }
    SPRITES = temp;

    SPRITES[size] = (Sprite *)sprite;
    SPRITES[size + 1] = NULL;
}

void edit_sprite_in_sprites(const int sprite_index) {

}
