//
// Created by niki on 5/14/26.
//

#ifndef STRUCT_FILE_DISPLAY_H
#define STRUCT_FILE_DISPLAY_H

#include "db_structures.h"

void display_sprites(const Sprite **sprites, ScreenType screen);
void display_sprite(const Sprite *sprite, ScreenType screen);
//void display_details(const Sprite *sprite, ScreenType screen);      //unnecessary, because display_sprite directly calls display_details
void display_text(const Sprite *sprite, ScreenType screen);
void display_line(const Sprite *sprite, ScreenType screen);
void display_slot(const Sprite *sprite, ScreenType screen);
void display_string(const char *str, int *x, int *y, ScreenType screen, int margin);

void output_screen(ScreenType screen);


void delete_screen(ScreenType *screen);

#endif //STRUCT_FILE_DISPLAY_H