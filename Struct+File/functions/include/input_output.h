//
// Created by niki on 5/13/26.
//

#ifndef STRUCT_FILE_INPUT_OUTPUT_H
#define STRUCT_FILE_INPUT_OUTPUT_H

#include <stdio.h>

#include "db_structures.h"

void input_sprite(Sprite *sprite);
void input_details(DetailsType *details, TypeOfSprite type);
void input_time(TimeType *time);

Sprite **input_sprites_from_file(const char *filename);
//the cursor in the file has to be right before the info about the necessary Sprite
void input_sprite_from_file(Sprite *sprite, FILE *source);
void input_spriteType_from_file(TypeOfSprite *type, FILE *source);
void input_details_from_file(DetailsType *details, TypeOfSprite type, FILE *source);
void input_direction_from_file(DirectionEnum *direction, FILE *source);
void input_status_from_file(SlotStatus *status, FILE *source);
void input_time_from_file(TimeType *time, FILE *source);

//work with stdout or custom file
void output_sprites(Sprite **sprites, FILE *dest);
void output_sprite(const Sprite *sprite, FILE *dest);
void output_spriteType(const TypeOfSprite *sprite, FILE *dest);
void output_details(const DetailsType *details, TypeOfSprite type, FILE *dest);
void output_direction(const DirectionEnum *direction, FILE *dest);
void output_status(const SlotStatus *status, FILE *dest);
void output_time(const TimeType *time, FILE *dest);

//binary
Sprite **input_sprites_binary(const char *file_name);
//void input_sprite_binary(Sprite *sprite, FILE *source);

void output_sprites_binary(Sprite **sprites, FILE *dest);
void output_sprite_binary(const Sprite *sprite, FILE *dest);

#endif //STRUCT_FILE_INPUT_OUTPUT_H