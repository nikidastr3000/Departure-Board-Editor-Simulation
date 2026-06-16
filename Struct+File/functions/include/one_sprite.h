//
// Created by niki on 5/13/26.
//

#ifndef STRUCT_FILE_ONE_SPRITE_H
#define STRUCT_FILE_ONE_SPRITE_H

#include <stdbool.h>

#include "globals_and_structures.h"

int measure_sprite_length(Sprite *sprite);

bool validate_sprite(const Sprite *sprite);

char *slot_to_string(const ScheduleSlot *slot, char bg_char);

#endif //STRUCT_FILE_ONE_SPRITE_H