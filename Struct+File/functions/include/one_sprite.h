//
// Created by niki on 5/13/26.
//

#ifndef STRUCT_FILE_ONE_SPRITE_H
#define STRUCT_FILE_ONE_SPRITE_H

#include "db_structures.h"

void set_actual_departure_time(Sprite *sprite, TimeType depTime);
//void bus_arrived(SpriteType *sprite, TimeType time);

int measure_sprite_length(Sprite *sprite);

bool validate_sprite(const Sprite *sprite);
bool validate_details(DetailsType *details, TypeOfSprite type);

char *slot_to_string(const ScheduleSlot *slot, char bg_char);

#endif //STRUCT_FILE_ONE_SPRITE_H