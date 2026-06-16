//
// Created by niki on 5/13/26.
//

#ifndef STRUCT_FILE_INPUT_OUTPUT_H
#define STRUCT_FILE_INPUT_OUTPUT_H

#include <stdio.h>
#include <stdbool.h>

#include "globals_and_structures.h"

//work with stdin
void input_sprite_from_stdin(Sprite *sprite);
void input_details_from_stdin(DetailsType *details, TypeOfSprite type);

void input_time_from_stdin(TimeType *time, char *output);

Sprite **input_sprites_from_file(const char *filename, bool *is_file_successfully_opened);
//the cursor in the file has to be right before the info about the necessary Sprite
bool input_sprite_from_file(Sprite *sprite, FILE *source);

bool input_spriteType_from_file(TypeOfSprite *type, FILE *source);

bool input_details_from_file(DetailsType *details, TypeOfSprite type, FILE *source);

bool input_direction_from_file(DirectionEnum *direction, FILE *source);

bool input_status_from_file(SlotStatus *status, FILE *source);

bool input_time_from_file(TimeType *time, FILE *source);

//special short output for the array of sprites
void output_sprites_to_stdout(Sprite **sprites);

//work with stdout or custom file
void output_sprites(Sprite **sprites, FILE *dest);
void output_sprite(const Sprite *sprite, FILE *dest);
void output_spriteType(const TypeOfSprite *sprite, FILE *dest);
void output_details(const DetailsType *details, TypeOfSprite type, FILE *dest);
void output_direction(const DirectionEnum *direction, FILE *dest);
void output_status(const SlotStatus *status, FILE *dest);
void output_time(const TimeType *time, FILE *dest);

/*
//binary
Sprite **input_sprites_binary(const char *file_name);
//void input_sprite_binary(Sprite *sprite, FILE *source);

void output_sprites_binary(Sprite **sprites, FILE *dest);
void output_sprite_binary(const Sprite *sprite, FILE *dest);
*/

void free_sprites_array(Sprite **sprites);
void free_sprite(Sprite *sprite);

#endif //STRUCT_FILE_INPUT_OUTPUT_H