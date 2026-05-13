//db - departure board


#ifndef STRUCT_FILE_DB_FUNCTIONS_H
#define STRUCT_FILE_DB_FUNCTIONS_H

#include <stdio.h>

#include "db_structures.h"

////////////////////info input/output functions////////////////////
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



////////////////////functions for 1 Sprite////////////////////
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


////////////////////functions for more than one Sprite////////////////////

bool check_for_mistakes(Sprite *sprites, ScreenBuffer *screen);



Sprite *find_sprites_by_model(Sprite *sprites, Sprite model);
void block_sprite_fields(Sprite *sprite);
bool cmp_sprite_with_model(Sprite *sprite, Sprite model);

Sprite *find_sprites_by_coords_diapason(Sprite *sprites, int x1, int y1, int x2, int y2);

bool check_collision(Sprite *sprite1, Sprite *sprite2);





#endif //STRUCT_FILE_DB_FUNCTIONS_H