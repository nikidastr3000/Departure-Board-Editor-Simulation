//
// Created by niki on 5/14/26.
//

#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "help_functions.h"
#include "one_sprite.h"

bool display_sprites(const Sprite **sprites, ScreenType *screen) {
    ScreenType *test_screen = check_sprites(sprites, *screen);
    if (test_screen == NULL) {
        return false;
    }
    delete_screen(test_screen);
    free(test_screen);

    for (int i = 0; sprites[i] != NULL; i++) {
        display_sprite(sprites[i], screen);
    }

    output_screen(screen, false);
    return true;
}

void display_sprite(const Sprite *sprite, ScreenType *screen) {
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

void display_text(const Sprite *sprite, ScreenType *screen) {
    const int y = sprite->y;
    const int x = sprite->x;
    for (int i = 0; sprite->details.text.content[i] != '\0' && x + i < screen->width && y < screen->height; i++) {
        screen->buffer[y][x + i] = sprite->details.text.content[i];
    }
}

void display_line(const Sprite *sprite, ScreenType *screen) {
    const int y = sprite->y;
    const int x = sprite->x;
    const char character = sprite->details.line.character;
    const int length = sprite->details.line.length;
    const DirectionEnum direction = sprite->details.line.direction;

    switch (direction) {
        case RIGHT:
            for (int i = 0; i < length && x + i < screen->width && y < screen->height; i++) {
                screen->buffer[y][x + i] = character;
            }
            break;
        case DOWN:
            for (int i = 0; i < length && y + i < screen->height && y < screen->height; i++) {
                screen->buffer[y + i][x] = character;
            }
            break;
    }
}

void display_slot(const Sprite *sprite, ScreenType *screen) {
    char *slot_str = slot_to_string(&sprite->details.slot, screen->bg_char);

    const int x = sprite->x;
    const int y = sprite->y;

    for (int i = 0; slot_str[i] != '\0' && x + i < screen->width && y < screen->height; i++) {
        if (slot_str[i] == screen->bg_char)         //skip if 'empty'
            continue;
        screen->buffer[y][x + i] = slot_str[i];
    }

    free(slot_str);
}





//screen functions

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


ScreenType *check_sprites(const Sprite **sprites, ScreenType screen) {
    printf("Checking sprites...\n");

    ScreenType *test_screen = malloc(sizeof(ScreenType));
    test_screen->height = screen.height;
    test_screen->width = screen.width;
    test_screen->bg_char = screen.bg_char;
    fill_screen(test_screen);

    bool allSpritesValid = true;

    for (int i = 0; sprites[i] != NULL; i++) {
        //checking each sprite if it's valid by him self
        if (!validate_sprite(sprites[i])) {
            printf("Sprite number %d(\"%s\") is invalid!\n", i, sprites[i]->name);
            printf("Firstly repair the sprite, then try again!\n\n");
            return NULL;
        }

        if (sprites[i]->x >= test_screen->width || sprites[i]->y >= test_screen->height) {
            printf("Sprite number %d(\"%s\") is outside the screen boundaries in coordinates (%d,%d)!\n",
            i, sprites[i]->name, sprites[i]->x, sprites[i]->y);
            printf("Firstly repair the sprite, then try again!\n\n");
            return NULL;
        }

        switch (sprites[i]->type) {
            case TEXT:
                if (!check_display_text(sprites, i, test_screen)) {
                    allSpritesValid = false;
                }
                break;
            case LINE:
                if (!check_display_line(sprites, i, test_screen)) {
                    allSpritesValid = false;
                }
                break;
            case SLOT:
                if (!check_display_slot(sprites, i, test_screen)) {
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

bool check_display_text(const Sprite **sprites, const int curr_sprite_num, ScreenType *test_screen) {
    bool ans = true;

    int y = sprites[curr_sprite_num]->y;
    int x = sprites[curr_sprite_num]->x;

    for (int j = 0; sprites[curr_sprite_num]->details.text.content[j] != '\0'; j++) {
        if (x >= test_screen->width) {
            print_screen_collision_warning(sprites, curr_sprite_num, x, y);
            ans = false;
            break;
        }

        if (test_screen->buffer[y][x] != test_screen->bg_char) {
            int second_sprite_num = test_screen->buffer[y][x] - '0';        //getting the number of the sprite that is already at (x+j,y)
            print_sprites_collision_warning(sprites, curr_sprite_num, second_sprite_num, x, y);
            ans = false;
        }

        test_screen->buffer[y][x] = curr_sprite_num + '0';
        x++;
    }

    return ans;
}

bool check_display_line(const Sprite **sprites, const int curr_sprite_num, ScreenType *test_screen) {
    bool ans = true;

    int y = sprites[curr_sprite_num]->y;
    int x = sprites[curr_sprite_num]->x;

    switch (sprites[curr_sprite_num]->details.line.direction) {
        case RIGHT:
            for (int j = 0; j < sprites[curr_sprite_num]->details.line.length; j++) {
                //checking for the screen border collision
                if (x >= test_screen->width) {
                    print_screen_collision_warning(sprites, curr_sprite_num, x, y);
                    ans = false;
                    break;
                }

                //checking for the sprite collision
                if (test_screen->buffer[y][x] != test_screen->bg_char) {
                    int second_sprite_num = test_screen->buffer[y][x] - '0';        //getting the number of the sprite that is already at (x+j,y)
                    print_sprites_collision_warning(sprites, curr_sprite_num, second_sprite_num, x, y);
                    ans = false;
                }

                test_screen->buffer[y][x] = curr_sprite_num + '0';
                x++;
            }
            break;
        case DOWN:
            for (int j = 0; j < sprites[curr_sprite_num]->details.line.length; j++) {
                //checking for the screen border collision
                if (y >= test_screen->height) {
                    print_screen_collision_warning(sprites, curr_sprite_num, x, y);
                    ans = false;
                    break;
                }

                //checking for the sprite collision
                if (test_screen->buffer[y][x] != test_screen->bg_char) {
                    int second_sprite_num = test_screen->buffer[y][x] - '0';        //getting the number of the sprite that is already at (x+j,y)
                    print_sprites_collision_warning(sprites, curr_sprite_num, second_sprite_num, x, y);
                    ans = false;
                }

                test_screen->buffer[y][x] = curr_sprite_num + '0';
                y++;
            }
            break;
    }

    return ans;
}

bool check_display_slot(const Sprite **sprites, const int curr_sprite_num, ScreenType *test_screen) {
    bool ans = true;

    char *slot_str = slot_to_string(&sprites[curr_sprite_num]->details.slot, test_screen->bg_char);
    int x = sprites[curr_sprite_num]->x;
    int y = sprites[curr_sprite_num]->y;

    for (int i = 0; slot_str[i] != '\0'; i++) {
        if (x >= test_screen->width) {
            print_screen_collision_warning(sprites, curr_sprite_num, x, y);
            ans = false;
            break;
        }

        if (slot_str[i] == test_screen->bg_char) {
            x++;
            continue;
        }

        if (test_screen->buffer[y][x] != test_screen->bg_char) {
            int second_sprite_num = test_screen->buffer[y][x] - '0';
            print_sprites_collision_warning(sprites, curr_sprite_num, second_sprite_num, x, y);
            ans = false;
        }

        test_screen->buffer[y][x] = curr_sprite_num + '0';
        x++;
    }

    return ans;
}


void print_screen_collision_warning(const Sprite **sprites, const int curr_sprite_num, const int x, const int y) {
    printf(" -Sprite number %d(\"%s\") extends beyond the screen boundaries in coordinates (%d,%d)!\n",
    curr_sprite_num, sprites[curr_sprite_num]->name, x, y);
}

void print_sprites_collision_warning(const Sprite **sprites, const int curr_sprite_num, const int second_sprite_num, const int x, const int y) {
    printf(" -Sprite number %d(\"%s\") collides with sprite number %d(\"%s\") in coordinates (%d,%d)!\n",
                        curr_sprite_num, sprites[curr_sprite_num]->name, second_sprite_num, sprites[second_sprite_num]->name, x, y);
}
