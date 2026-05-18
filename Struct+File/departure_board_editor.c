//
// Created by niki on 5/17/26.
//

#include "departure_board_editor.h"

#include <stdlib.h>

int MAX_STRING_SIZE;
int SLOT_MARGIN;
ScreenType SCREEN;
StateEnum STATE;
char *OPENED_FILE_NAME;
Sprite **SPRITES;

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

    STATE = IN_START_MODE;

    OPENED_FILE_NAME = malloc(MAX_STRING_SIZE);

    SPRITES = NULL;

    fclose(config_file);
}

void compute_state() {
    switch (STATE) {
        case OPENING_FILE:
            puts("Opening file...");

            printf("Enter File Name: ");
            scanf(" %s", OPENED_FILE_NAME);

            SPRITES = input_sprites_from_file(OPENED_FILE_NAME);

            STATE = IN_EDIT_MODE;

            break;

        case CREATING_FILE:
            puts("Creating file...");

            printf("Enter File Name: ");
            scanf(" %s", OPENED_FILE_NAME);

            FILE *file = fopen(OPENED_FILE_NAME, "w");
            if (file == NULL) {
                puts("Couldn't create file!");
                exit(1);
            }
            fclose(file);

            STATE = IN_EDIT_MODE;

            break;

        case EXITING_PROGRAM:
            puts("Exiting program...");

            if (SPRITES != NULL) {
                free_sprites_array(SPRITES);
            }

            break;

        case SAVING_FILE:
            puts("Saving file...");

            FILE *fp = fopen(OPENED_FILE_NAME, "w");
            output_sprites(SPRITES, fp);
            fclose(fp);

            STATE = IN_EDIT_MODE;

            break;

        case CLOSING_FILE:
            puts("Closing file...");

            free_sprites_array(SPRITES);
            SPRITES = NULL;

            OPENED_FILE_NAME[0] = '\0';

            STATE = IN_START_MODE;

            break;

        case DISPLAYING:
            puts("Displaying...");

            display_sprites();

            STATE = IN_EDIT_MODE;

            break;

        case ADDING_SPRITE:
            puts("Adding sprite...");

            Sprite *sprite = malloc(sizeof(Sprite));
            input_sprite_from_stdin(sprite);

            add_sprite_to_sprites(sprite);

            STATE = IN_EDIT_MODE;

            break;

        case EDITING_SPRITE:
            puts("Editing sprite...");

            Sprite *model_sprite = malloc(sizeof(Sprite));
            input_sprite_from_stdin(model_sprite);
            edit_sprite_in_sprites(model_sprite);
            free(model_sprite);

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
        switch (STATE) {
            case IN_START_MODE:
                puts("(1) Open File");
                puts("(2) Create new File");
                puts("(3) Exit");
                printf("Enter your choice: ");
                scanf(" %d", &STATE);
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
                printf("Enter your choice: ");
                scanf(" %d", &STATE);
                break;

            case EXITING_PROGRAM:
                flag = false;
                break;
            default:
                puts("Invalid state!");
                exit(1);
        }

        if (!flag) {
            break;
        }

        compute_state();
    }
}

void start() {
    init();

    main_loop();
}