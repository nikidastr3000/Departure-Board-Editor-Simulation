//
// Created by niki on 5/14/26.
//

#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "help_functions.h"
#include "one_sprite.h"

bool display_sprites(const bool test_mode) {
    if (SPRITES == NULL || SPRITES[0] == NULL) {
        printf("No sprites to display!\n");
        output_screen(&SCREEN, false);
        return false;
    }

    ScreenType *test_screen = check_sprites(SCREEN);
    if (test_screen == NULL) {
        return false;
    }
    delete_screen(test_screen);
    free(test_screen);

    clear_screen(&SCREEN);
    for (int i = 0; SPRITES[i] != NULL; i++) {
        display_sprite(SPRITES[i]);
    }

    output_screen(&SCREEN, test_mode);
    return true;
}

void display_sprite(const Sprite *sprite) {
    switch (sprite->type) {
        case TEXT:
            display_text(sprite);
            break;
        case LINE:
            display_line(sprite);
            break;
        case SLOT:
            display_slot(sprite);
            break;
    }
}

void display_text(const Sprite *sprite) {
    const int y = sprite->y;
    const int x = sprite->x;
    
    for (int i = 0; sprite->details.text.content[i] != '\0' && x + i < SCREEN.width && y < SCREEN.height; i++) {
        SCREEN.buffer[y][x + i] = sprite->details.text.content[i];
    }
}

void display_line(const Sprite *sprite) {
    const int y = sprite->y;
    const int x = sprite->x;
    const char character = sprite->details.line.character;
    const int length = sprite->details.line.length;
    const DirectionEnum direction = sprite->details.line.direction;

    switch (direction) {
        case RIGHT:
            for (int i = 0; i < length && x + i < SCREEN.width && y < SCREEN.height; i++) {
                SCREEN.buffer[y][x + i] = character;
            }
            break;
        case DOWN:
            for (int i = 0; i < length && y + i < SCREEN.height && y < SCREEN.height; i++) {
                SCREEN.buffer[y + i][x] = character;
            }
            break;
    }
}

void display_slot(const Sprite *sprite) {
    char *slot_str = slot_to_string(&sprite->details.slot, SCREEN.bg_char);

    const int x = sprite->x;
    const int y = sprite->y;

    for (int i = 0; slot_str[i] != '\0' && x + i < SCREEN.width && y < SCREEN.height; i++) {
        if (slot_str[i] == SCREEN.bg_char)         //skip if 'empty'
            continue;
        SCREEN.buffer[y][x + i] = slot_str[i];
    }

    free(slot_str);
}





//SCREEN functions

void fill_screen(ScreenType *screen) {
    screen->buffer = (char **) malloc(sizeof(char *) * screen->height);
    for (int i = 0; i < screen->height; i++) {
        screen->buffer[i] = (char *) malloc(sizeof(char) * (screen->width + 1));
        for (int j = 0; j < screen->width; j++) {
            screen->buffer[i][j] = screen->bg_char;
        }
        screen->buffer[i][screen->width] = '\0';
    }
}

void output_screen(const ScreenType *screen, bool test_mode) {
    // max possible number of digits in x and y coordinates
    int max_digits_in_x = int_string_length(screen->width - 1);      //-1 because coords start from 0
    int max_digits_in_y = int_string_length(screen->height - 1);     //-1 because coords start from 0

    if (test_mode) {        //x roods
        char x_axis[max_digits_in_x][screen->width + 1];         //+1 for '\0'
        for (int i = 0; i < screen->width; i++) {
            char curr_coord[max_digits_in_x];       //number of coord
            sprintf(curr_coord, "%*d", max_digits_in_x, i);
            for (int j = 0; j < max_digits_in_x; j++) {
                   x_axis[j][i] = curr_coord[j];    //saving number vertically
            }
        }
        for (int i = 0; i < max_digits_in_x; i++) {
            x_axis[i][screen->width] = '\0';
        }

        for (int i = 0; i < max_digits_in_x; i++) {
            printf("%*s ", max_digits_in_y, "");    //spaces for y-axis
            printf("%s\n", x_axis[i]);
        }
        //puts("");
    }

    for (int i = 0; i < screen->height; i++) {
        if (test_mode) {
            printf("%*d ", max_digits_in_y, i);
        }

        printf("%s\n", screen->buffer[i]);
    }
    puts("");
}

void delete_screen(ScreenType *screen) {
    for (int i = 0; i < screen->height; i++) {
        free(screen->buffer[i]);
    }
    free(screen->buffer);
}

void clear_screen(ScreenType *screen) {
    for (int i = 0; i < screen->height; i++) {
        for (int j = 0; j < screen->width; j++) {
            screen->buffer[i][j] = screen->bg_char;
        }
    }
}



ScreenType *check_sprites(ScreenType screen) {
    ScreenType *test_screen = malloc(sizeof(ScreenType));
    test_screen->height = screen.height;
    test_screen->width = screen.width;
    test_screen->bg_char = screen.bg_char;
    fill_screen(test_screen);

    if (SPRITES == NULL || SPRITES[0] == NULL) {
        printf("No sprites to check!\n");
        return test_screen;
    }

    printf("Checking sprites...\n");

    bool allSpritesValid = true;

    for (int i = 0; SPRITES[i] != NULL; i++) {
        //checking each sprite if it's valid by him self
        if (!validate_sprite(SPRITES[i])) {
            printf("Sprite number %d(\"%s\") is invalid!\n", i, SPRITES[i]->name);
            printf("Firstly repair the sprite, then try again!\n\n");
            delete_screen(test_screen);
            free(test_screen);
            return NULL;
        }

        if (SPRITES[i]->x >= test_screen->width || SPRITES[i]->y >= test_screen->height) {
            printf("Sprite number %d(\"%s\") is outside the SCREEN boundaries in coordinates (%d,%d)!\n",
            i, SPRITES[i]->name, SPRITES[i]->x, SPRITES[i]->y);
            printf("Firstly repair the sprite, then try again!\n\n");
            delete_screen(test_screen);
            free(test_screen);
            return NULL;
        }

        switch (SPRITES[i]->type) {
            case TEXT:
                if (!check_display_text(i, test_screen)) {
                    allSpritesValid = false;
                }
                break;
            case LINE:
                if (!check_display_line(i, test_screen)) {
                    allSpritesValid = false;
                }
                break;
            case SLOT:
                if (!check_display_slot(i, test_screen)) {
                    allSpritesValid = false;
                }
                break;
        }
    }

    if (allSpritesValid) {
        printf("All sprites are valid!\n");
    }
    puts("");
    return test_screen;
}

bool check_display_text(const int curr_sprite_num, ScreenType *test_screen) {
    bool ans = true;

    int y = SPRITES[curr_sprite_num]->y;
    int x = SPRITES[curr_sprite_num]->x;

    for (int j = 0; SPRITES[curr_sprite_num]->details.text.content[j] != '\0'; j++) {
        if (x >= test_screen->width) {
            print_screen_collision_warning(curr_sprite_num, x, y);
            ans = false;
            break;
        }

        if (test_screen->buffer[y][x] != test_screen->bg_char) {
            int second_sprite_num = test_screen->buffer[y][x] - '0';        //getting the number of the sprite that is already at (x+j,y)
            print_sprites_collision_warning(curr_sprite_num, second_sprite_num, x, y);
            ans = false;
        }

        test_screen->buffer[y][x] = curr_sprite_num + '0';
        x++;
    }

    return ans;
}

bool check_display_line(const int curr_sprite_num, ScreenType *test_screen) {
    bool ans = true;

    int y = SPRITES[curr_sprite_num]->y;
    int x = SPRITES[curr_sprite_num]->x;

    switch (SPRITES[curr_sprite_num]->details.line.direction) {
        case RIGHT:
            for (int j = 0; j < SPRITES[curr_sprite_num]->details.line.length; j++) {
                //checking for the SCREEN border collision
                if (x >= test_screen->width) {
                    print_screen_collision_warning(curr_sprite_num, x, y);
                    ans = false;
                    break;
                }

                //checking for the sprite collision
                if (test_screen->buffer[y][x] != test_screen->bg_char) {
                    int second_sprite_num = test_screen->buffer[y][x] - '0';        //getting the number of the sprite that is already at (x+j,y)
                    print_sprites_collision_warning(curr_sprite_num, second_sprite_num, x, y);
                    ans = false;
                }

                test_screen->buffer[y][x] = curr_sprite_num + '0';
                x++;
            }
            break;
        case DOWN:
            for (int j = 0; j < SPRITES[curr_sprite_num]->details.line.length; j++) {
                //checking for the SCREEN border collision
                if (y >= test_screen->height) {
                    print_screen_collision_warning(curr_sprite_num, x, y);
                    ans = false;
                    break;
                }

                //checking for the sprite collision
                if (test_screen->buffer[y][x] != test_screen->bg_char) {
                    int second_sprite_num = test_screen->buffer[y][x] - '0';        //getting the number of the sprite that is already at (x+j,y)
                    print_sprites_collision_warning(curr_sprite_num, second_sprite_num, x, y);
                    ans = false;
                }

                test_screen->buffer[y][x] = curr_sprite_num + '0';
                y++;
            }
            break;
    }

    return ans;
}

bool check_display_slot(const int curr_sprite_num, ScreenType *test_screen) {
    bool ans = true;

    char *slot_str = slot_to_string(&SPRITES[curr_sprite_num]->details.slot, test_screen->bg_char);
    int x = SPRITES[curr_sprite_num]->x;
    int y = SPRITES[curr_sprite_num]->y;

    for (int i = 0; slot_str[i] != '\0'; i++) {
        if (x >= test_screen->width) {
            print_screen_collision_warning(curr_sprite_num, x, y);
            ans = false;
            break;
        }

        if (slot_str[i] == test_screen->bg_char) {
            x++;
            continue;
        }

        if (test_screen->buffer[y][x] != test_screen->bg_char) {
            int second_sprite_num = test_screen->buffer[y][x] - '0';
            print_sprites_collision_warning(curr_sprite_num, second_sprite_num, x, y);
            ans = false;
        }

        test_screen->buffer[y][x] = curr_sprite_num + '0';
        x++;
    }

    free(slot_str);
    return ans;
}


void print_screen_collision_warning(const int curr_sprite_num, const int x, const int y) {
    printf(" -Sprite number %d(\"%s\") extends beyond the SCREEN boundaries in coordinates (%d,%d)!\n",
    curr_sprite_num, SPRITES[curr_sprite_num]->name, x, y);
}

void print_sprites_collision_warning(const int curr_sprite_num, const int second_sprite_num, const int x, const int y) {
    printf(" -Sprite number %d(\"%s\") collides with sprite number %d(\"%s\") in coordinates (%d,%d)!\n",
                        curr_sprite_num, SPRITES[curr_sprite_num]->name, second_sprite_num, SPRITES[second_sprite_num]->name, x, y);
}