//
// Created by niki on 5/13/26.
//

#include "many_sprites.h"

#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "input_output.h"

bool add_sprite_to_sprites(const Sprite *sprite) {
    int len = sprites_len(SPRITES);

    Sprite **temp = realloc(SPRITES, (len + 2) * sizeof(Sprite *));     //sprites[size+1] will be the last sprite (=NULL)
    if (temp == NULL) {
        perror("realloc");
        puts("Error! Sprite not added!");
        /*FILE *fp = fopen("last_usage.txt", "w");
        output_sprites(SPRITES, fp);
        fclose(fp);*/
        return false;
    }
    SPRITES = temp;

    SPRITES[len] = (Sprite *)sprite;
    SPRITES[len + 1] = NULL;

    printf("Sprite \"%s\" added!\n", sprite->name);
    return true;
}

bool edit_sprite_in_sprites(const int sprite_index) {

}

bool delete_sprite_from_sprites(int sprite_index) {
    int len = sprites_len(SPRITES);

    if (sprite_index < 0 || sprite_index >= len) {
        printf("Invalid sprite index!\n");
        return false;
    }

    for (int i = sprite_index; i < len - 1; i++) {
        SPRITES[i] = SPRITES[i + 1];
    }
    free_sprite(SPRITES[len - 1]);
    SPRITES[len - 1] = NULL;

    printf("Sprite deleted!\n");
    return true;
}

int sprites_len(Sprite **sprites) {
    int len = 0;
    for (; sprites[len] != NULL; len++) {}
    return len;
}