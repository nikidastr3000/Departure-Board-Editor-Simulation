//
// Created by niki on 5/13/26.
//

#ifndef STRUCT_FILE_ONE_SPRITE_H
#define STRUCT_FILE_ONE_SPRITE_H

#include "db_structures.h"

void display_sprite(Sprite *sprite);
void display_details(DetailsType *details, TypeOfSprite type);
void display_text(Sprite *sprite);
void display_line(Sprite *sprite);
void display_slot(Sprite *sprite);


void set_actual_departure_time(Sprite *sprite, TimeType depTime);
//void bus_arrived(SpriteType *sprite, TimeType time);

int measure_sprite_length(Sprite *sprite);

bool check_sprite(Sprite *sprite);
bool check_details(DetailsType *details, TypeOfSprite type);

#endif //STRUCT_FILE_ONE_SPRITE_H