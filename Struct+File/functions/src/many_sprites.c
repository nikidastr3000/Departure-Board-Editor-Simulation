//
// Created by niki on 5/13/26.
//

#include "many_sprites.h"

#include <stdlib.h>

void delete_sprites_array(Sprite **sprites) {
    for (int i = 0; sprites[i] != NULL; i++) {
        free(sprites[i]);
    }
    free(sprites);
}
