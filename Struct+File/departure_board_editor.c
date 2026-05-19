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

    STATE = NO_OPENED_FILE;

    OPENED_FILE_NAME = malloc(MAX_STRING_SIZE);
    OPENED_FILE_NAME[0] = '\0';

    SPRITES = NULL;

    fclose(config_file);

    /*//
    OPENED_FILE_NAME = "sprites.txt";
    SPRITES = input_sprites_from_file(OPENED_FILE_NAME);
    STATE = FILE_OPENED;
    //*/
}


bool compute_state() {
    printf("\n");
    switch (STATE) {
        case OPENING_FILE:
            puts("Opening file...");

            printf("Enter File Name: ");
            scanf(" %s", OPENED_FILE_NAME);
            clear_stdin();

            bool is_file_successfully_opened = false;
            SPRITES = input_sprites_from_file(OPENED_FILE_NAME, &is_file_successfully_opened);
            if (is_file_successfully_opened) {
                STATE = FILE_OPENED;
            }
            else{
                STATE = NO_OPENED_FILE;
            }

            puts("File successfully opened!");

            break;

        case CREATING_FILE:
            puts("Creating file...");

            printf("Enter File Name: ");
            scanf(" %s", OPENED_FILE_NAME);
            clear_stdin();

            FILE *file = fopen(OPENED_FILE_NAME, "w");
            if (file == NULL) {
                puts("Couldn't create file!");
                exit(1);
            }
            fclose(file);

            puts("File created!");

            STATE = FILE_OPENED;

            break;

        case EXITING_PROGRAM:
            puts("Exiting program...");

            if (SPRITES != NULL) {
                free_sprites_array(SPRITES);
            }

            delete_screen(&SCREEN);

            free(OPENED_FILE_NAME);

            return false;

        case SAVING_FILE:
            puts("Saving file...");

            FILE *fp_saving = fopen(OPENED_FILE_NAME, "w");
            output_sprites(SPRITES, fp_saving);
            fclose(fp_saving);

            puts("File saved!");

            STATE = FILE_OPENED;

            break;

        case CLOSING_FILE:
            puts("Closing file...");

            FILE *fp_closure = fopen(OPENED_FILE_NAME, "w");
            output_sprites(SPRITES, fp_closure);
            fclose(fp_closure);
            puts("File saved!");

            free_sprites_array(SPRITES);
            SPRITES[0] = NULL;

            OPENED_FILE_NAME[0] = '\0';

            STATE = NO_OPENED_FILE;

            break;

        case DISPLAYING:
            puts("Displaying...");

            display_sprites();

            STATE = FILE_OPENED;

            break;

        case OUTPUTTING_ALL_SPRITES_INFO:
            puts("Outputting all sprites info...");

            output_sprites_to_stdout(SPRITES);

            STATE = FILE_OPENED;

            break;

        case OUTPUTTING_SPRITE_INFO:
            puts("Outputting sprite info...");

            int sprite_index_for_output;
            printf("Enter sprite index: ");
            scanf(" %d", &sprite_index_for_output);
            clear_stdin();
            output_sprite(SPRITES[sprite_index_for_output], stdout);

            STATE = FILE_OPENED;

            break;

        case ADDING_SPRITE:
            puts("Adding sprite...");

            Sprite *sprite = malloc(sizeof(Sprite));
            input_sprite_from_stdin(sprite);

            add_sprite_to_sprites(sprite);

            STATE = FILE_OPENED;

            break;

        case EDITING_SPRITE:
            puts("Editing sprite...");

            int sprite_index_for_edit;
            printf("Enter sprite index: ");
            scanf(" %d", &sprite_index_for_edit);
            clear_stdin();

            edit_sprite_in_sprites(sprite_index_for_edit);

            STATE = FILE_OPENED;


            break;

        case DELETING_SPRITE:
            puts("Deleting sprite...");

            int sprite_index_for_delete;
            printf("Enter sprite index: ");
            scanf(" %d", &sprite_index_for_delete);
            clear_stdin();

            printf("Are you sure? (y/n): ");
            char answer;
            scanf(" %c", &answer);
            clear_stdin();
            if (answer == 'y') {
                delete_sprite_from_sprites(sprite_index_for_delete);
            }

            STATE = FILE_OPENED;
            break;

        default:
            puts("Invalid state!");
            STATE = NO_OPENED_FILE;
    }

    printf("\n");
    return true;
}

static void main_loop() {
    while (true) {
        switch (STATE) {
            case NO_OPENED_FILE:
                puts("(1) Open Board");
                puts("(2) Create new Board");
                puts("(3) Exit");
                printf("Enter your choice: ");

                break;

            case FILE_OPENED:
                puts("F");
                puts("(3) Exit");

                puts("(1) Open Board");
                puts("(2) Create new Board");

                puts("(4) Save Changes");
                puts("(5) Close Board");

                //EDIT
                puts("(6) Display");
                puts("(7) Output all sprites info");
                puts("(8) Output sprite info");
                puts("(9) Add Sprite");
                puts("(10) Edit Sprite");
                puts("(11) Delete Sprite");
                printf("Enter your choice: ");

                break;

            default:
                puts("Invalid state!");
                exit(1);
        }

        scanf(" %d", &STATE);
        clear_stdin();

        if (STATE == EXITING_PROGRAM) {
            break;
        }

        compute_state();
    }
}

void start() {
    init();

    main_loop();
}