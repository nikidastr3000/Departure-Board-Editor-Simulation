//
// Created by niki on 5/13/26.
//

#ifndef STRUCT_FILE_MANY_SPRITES_H
#define STRUCT_FILE_MANY_SPRITES_H

#include "globals_and_structures.h"

void add_sprite_to_sprites(const Sprite *sprite);

void edit_sprite_in_sprites(int sprite_index);



Sprite *find_sprites_by_model(Sprite **sprites, Sprite model);
void block_sprite_fields(Sprite *sprite);
bool cmp_sprite_with_model(Sprite *sprite, Sprite model);

Sprite *find_sprites_by_coords_diapason(Sprite *sprites, int x1, int y1, int x2, int y2);

//bool check_collision(Sprite *sprite1, Sprite *sprite2);

#endif //STRUCT_FILE_MANY_SPRITES_H