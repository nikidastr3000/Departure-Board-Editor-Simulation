//
// Created by niki on 5/17/26.
//

#include "departure_board_editor.h"

#include <stdlib.h>

int MAX_STRING_SIZE;
int SLOT_MARGIN;
ScreenType SCREEN;
StateEnum state;
char *opened_file_name;
Sprite **sprites;

static void init() {
    FILE *config_file = fopen("config.txt", "r");
    if (config_file == NULL) {
        puts("Couldn't open config file!");
        exit(1);
    }

    find_field_in_file(config_file, "MAX_STRING_SIZE:");
    fscanf(config_file, " %d", &MAX_STRING_SIZE);

    find_field_in_file(config_file, "SLOT_MARGIN:");
    fscanf(config_file, " %d", &SLOT_MARGIN);

    find_field_in_file(config_file, "SCREEN_WIDTH:");
    fscanf(config_file, " %d", &SCREEN.width);

    find_field_in_file(config_file, "SCREEN_HEIGHT:");
    fscanf(config_file, " %d", &SCREEN.height);

    find_field_in_file(config_file, "SCREEN_BG_CHAR:");
    fscanf(config_file, " %d", &SCREEN.bg_char);

    fill_screen(&SCREEN);

    state = IN_START_MODE;

    opened_file_name = malloc(MAX_STRING_SIZE);

    sprites = NULL;

    fclose(config_file);
}

void compute_state() {
    switch (state) {
        case OPENING_FILE:
            puts("Opening file...");

            printf("Enter File Name: ");
            scanf(" %s", opened_file_name);

            sprites = input_sprites_from_file(opened_file_name);

            state = IN_EDIT_MODE;

            break;

        case CREATING_FILE:
            puts("Creating file...");

            printf("Enter File Name: ");
            scanf(" %s", opened_file_name);

            FILE *file = fopen(opened_file_name, "w");
            if (file == NULL) {
                puts("Couldn't create file!");
                exit(1);
            }
            fclose(file);

            state = IN_EDIT_MODE;

            break;

        case EXITING_PROGRAM:
            puts("Exiting program...");

            if (sprites != NULL) {
                free_sprites_array(sprites);
            }

            break;

        case SAVING_FILE:
            puts("Saving file...");

            FILE *fp = fopen(opened_file_name, "w");
            output_sprites(sprites, fp);
            fclose(fp);

            state = IN_EDIT_MODE;

            break;

        case CLOSING_FILE:
            puts("Closing file...");

            free_sprites_array(sprites);
            sprites = NULL;

            opened_file_name[0] = '\0';

            state = IN_START_MODE;

            break;

        case DISPLAYING:
            puts("Displaying...");

            display_sprites();

            state = IN_EDIT_MODE;

            break;

        case ADDING_SPRITE:
            puts("Adding sprite...");

            Sprite *sprite = malloc(sizeof(Sprite));
            input_sprite_from_stdin(sprite);

            sprites = add_sprite_to_array(sprites, sprite);


            state = IN_EDIT_MODE;

            break;

        case EDITING_SPRITE:
            puts("Editing sprite...");

            break;

        case DELETING_SPRITE:
            puts("Deleting sprite...");

            break;

        default:
            puts("Invalid state!");
            exit(1);
    }
}

static void main_loop() {
    bool flag = true;
    while (flag) {
        switch (state) {
            case IN_START_MODE:
                puts("(1) Open File");
                puts("(2) Create new File");
                puts("(3) Exit");
                puts("Enter your choice: ");
                scanf(" %d", &state);
                break;

            case IN_EDIT_MODE:
                puts("(1) Open File");
                puts("(2) Create new File");
                puts("(3) Exit");
                puts("(4) Save File");
                puts("(5) Close File");
                puts("(6) Display");
                puts("(7) Add Sprite");
                puts("(8) Edit Sprite");
                puts("(9) Delete Sprite");
                puts("Enter your choice: ");
                scanf(" %d", &state);
                break;

            case EXITING_PROGRAM:
                flag = false;
                break;
            default:
                puts("Invalid state!");
                exit(1);
        }
        compute_state();
    }
}

void start() {
    init();

    main_loop();
}