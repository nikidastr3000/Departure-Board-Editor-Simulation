//
// Created by niki on 5/14/26.
//

#ifndef STRUCT_FILE_DISPLAY_H
#define STRUCT_FILE_DISPLAY_H

#include "globals_and_structures.h"

bool display_sprites();
void display_sprite(const Sprite *sprite);
//void display_details(const Sprite *sprite, ScreenType SCREEN);      //unnecessary, because display_sprite directly calls display_details
void display_text(const Sprite *sprite);
void display_line(const Sprite *sprite);
void display_slot(const Sprite *sprite);


//SCREEN functions
void fill_screen(ScreenType *screen);
//test_mode - outputting SCREEN with coords system
void output_screen(const ScreenType *screen, bool test_mode);
void delete_screen(ScreenType *screen);


ScreenType *check_sprites(ScreenType screen);
bool check_display_text(int curr_sprite_num, ScreenType *test_screen);
bool check_display_line(int curr_sprite_num, ScreenType *test_screen);
bool check_display_slot(int curr_sprite_num, ScreenType *test_screen);

void print_screen_collision_warning(int curr_sprite_num, int x, int y);
void print_sprites_collision_warning(int curr_sprite_num,  int second_sprite_num,  int x,  int y);


#endif //STRUCT_FILE_DISPLAY_H