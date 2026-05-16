//
// Created by niki on 5/14/26.
//

#ifndef STRUCT_FILE_DISPLAY_H
#define STRUCT_FILE_DISPLAY_H

#include "db_structures.h"

void display_sprites(const Sprite **sprites, ScreenType *screen);
void display_sprite(const Sprite *sprite, ScreenType *screen);
//void display_details(const Sprite *sprite, ScreenType screen);      //unnecessary, because display_sprite directly calls display_details
void display_text(const Sprite *sprite, ScreenType *screen);
void display_line(const Sprite *sprite, ScreenType *screen);
void display_slot(const Sprite *sprite, ScreenType *screen);


//screen functions
void fill_screen(ScreenType *screen);
void output_screen(ScreenType screen, bool test_mode);
void delete_screen(ScreenType *screen);


bool check_sprites(const Sprite **sprites, ScreenType *test_screen);
bool check_display_text(const Sprite **sprites, int curr_sprite_num, ScreenType *test_screen);
bool check_display_line(const Sprite **sprites, int curr_sprite_num, ScreenType *test_screen);
bool check_display_slot(const Sprite **sprites, int curr_sprite_num, ScreenType *test_screen);

void print_screen_collision_warning(const Sprite **sprites, int curr_sprite_num, int x, int y);
void print_sprites_collision_warning(const Sprite **sprites,  int curr_sprite_num,  int second_sprite_num,  int x,  int y);


#endif //STRUCT_FILE_DISPLAY_H