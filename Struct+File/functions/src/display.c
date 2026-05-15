//
// Created by niki on 5/14/26.
//

#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_sprites(const Sprite **sprites, ScreenType screen) {
    for (int i = 0; sprites[i] != NULL; i++) {
        display_sprite(sprites[i], screen);
    }
}

void display_sprite(const Sprite *sprite, ScreenType screen) {
    switch (sprite->type) {
        case TEXT:
            display_text(sprite, screen);
            break;
        case LINE:
            display_line(sprite, screen);
            break;
        case SLOT:
            display_slot(sprite, screen);
            break;
    }
}

void display_text(const Sprite *sprite, ScreenType screen) {
    const int y = sprite->y;
    const int x = sprite->x;
    for (int i = 0; sprite->details.text.content[i] != '\0' && x + i < screen.width; i++) {
        screen.buffer[y][x + i] = sprite->details.text.content[i];
    }
}

void display_line(const Sprite *sprite, ScreenType screen) {
    const int y = sprite->y;
    const int x = sprite->x;
    const char character = sprite->details.line.character;
    const int length = sprite->details.line.length;
    const DirectionEnum direction = sprite->details.line.direction;

    switch (direction) {
        case RIGHT:
            for (int i = 0; i < length && x + i < screen.width; i++) {
                screen.buffer[y][x + i] = character;
            }
            break;
        case DOWN:
            for (int i = 0; i < length && y + i < screen.height; i++) {
                screen.buffer[y + i][x] = character;
            }
            break;
    }
}

void display_slot(const Sprite *sprite, ScreenType screen) {
    char station_number[5];             //max 4 digit + 1 char for '\0'
    sprintf(station_number, "%d", sprite->details.slot.station_number);

    char scheduled_departure[6];        //5 chars for "hh:mm" +1 for '\0'
    sprintf(scheduled_departure, "%02d:%02d", sprite->details.slot.scheduled_departure.hours, sprite->details.slot.scheduled_departure.minutes);

    char estimated_departure[6];        //5 chars for "hh:mm" +1 for '\0'
    sprintf(estimated_departure, "%02d:%02d", sprite->details.slot.estimated_departure.hours, sprite->details.slot.estimated_departure.minutes);

    char trip_number[5];                //max 4 digit + 1 char for '\0'
    sprintf(trip_number, "%d", sprite->details.slot.station_number);

    char status[MAX_STRING_SIZE];
    switch (sprite->details.slot.status) {
        case WAITING:
            strcpy(status, "WAITING");
            break;
        case IN_PROGRESS:
            strcpy(status, "IN_PROGRESS");
            break;
        case CANCELLED:
            strcpy(status, "CANCELLED");
            break;
    }

    int y = sprite->y;
    int x = sprite->x;

    display_string(station_number, &x, &y, screen, 1);
    display_string(scheduled_departure, &x, &y, screen, 1);
    display_string(estimated_departure, &x, &y, screen, 1);
    display_string(trip_number, &x, &y, screen, 1);
    display_string(status, &x, &y, screen, 1);
}

void display_string(const char *str, int *x, int *y, ScreenType screen, const int margin) {
    for (int i = 0; i < margin && *x + 1 < screen.width; i++) {
        screen.buffer[(*y)][*x + i] = ' ';
    }
    *x += margin;

    for (int i = 0; str[i] != '\0' && *x + i < screen.width; i++) {
        screen.buffer[(*y)][*x + i] = str[i];

    }

    for (int i = 0; i < margin && *x + 1 < screen.width; i++) {
        screen.buffer[(*y)][*x + i] = ' ';
    }
}


void output_screen(ScreenType screen) {
    for (int i = 0; i < screen.height; i++) {
        printf("%s\n", screen.buffer[i]);
    }
}



void delete_screen(ScreenType *screen) {
    for (int i = 0; i < screen->height; i++) {
        free(screen->buffer[i]);
    }
    free(screen->buffer);
}