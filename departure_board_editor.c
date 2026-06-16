//
// Created by niki on 5/17/26.
//

#include "departure_board_editor.h"

#include <stdlib.h>
#include <string.h>

int MAX_STRING_SIZE;
int SLOT_MARGIN;
ScreenType SCREEN;
StateType STATE;
char *OPENED_FILE_NAME;
Sprite **SPRITES;
int edit_sprite_number;

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
    int bg_char;
    fscanf(config_file, " %d", &bg_char);
    SCREEN.bg_char = (char)bg_char;

    fill_screen(&SCREEN);

    STATE.current_menu = NO_OPENED_FILE;
    STATE.current_action = NO_ACTION;

    OPENED_FILE_NAME = malloc(MAX_STRING_SIZE);
    OPENED_FILE_NAME[0] = '\0';

    SPRITES = NULL;

    edit_sprite_number = -1;

    fclose(config_file);

    /*//
    strcpy(OPENED_FILE_NAME, "sprites.txt");
    bool is_file_successfully_opened = false;
    SPRITES = input_sprites_from_file(OPENED_FILE_NAME, &is_file_successfully_opened);
    STATE.current_menu = FILE_OPENED;
    puts("");
    //*/
}

static void save_file() {
    puts("Saving file...");

    FILE *fp = fopen(OPENED_FILE_NAME, "w");
    output_sprites(SPRITES, fp);
    fclose(fp);

    puts("File saved!");
}

static void close_file() {
    puts("Closing file...");

    if (SPRITES != NULL) {
        free_sprites_array(SPRITES);
        SPRITES = NULL;
    }
    OPENED_FILE_NAME[0] = '\0';

    puts("File successfully closed!");
}

static void menu() {
    int choice;
    switch (STATE.current_menu) {
        case NO_OPENED_FILE:
            puts("========Start Menu========");
            puts("(1) Open Board");
            puts("(2) Create new Board");
            puts("(3) Exit");


            printf("Enter your choice: ");
            scanf(" %d", &choice);
            clear_stdin();

            switch (choice) {
                case 1:
                    STATE.current_action = OPENING_FILE;
                    break;
                case 2:
                    STATE.current_action = CREATING_NEW_FILE;
                    break;
                case 3:
                    STATE.current_action = EXITING_PROGRAM;
                    break;
                default:
                    puts("Invalid choice!");
            }

            break;

        case FILE_OPENED:
            puts("========Main Menu========");
            puts("(1) File Menu");
            puts("(2) Actions Menu");
            puts("(3) Exit");

            printf("Enter your choice: ");
            scanf(" %d", &choice);
            clear_stdin();

            switch (choice) {
                case 1:
                    STATE.current_menu= FILE_MENU;
                    break;
                case 2:
                    STATE.current_menu = ACTION_MENU;
                    break;
                case 3:
                    STATE.current_action = EXITING_PROGRAM;
                    break;
                default:
                    puts("Invalid choice!");
            }

            break;

        case FILE_MENU:
            puts("========File Menu========");
            puts("(1) Open Board");
            puts("(2) Create new Board");

            puts("(3) Save Changes");
            puts("(4) Close Board");

            puts("(5) return back");

            printf("Enter your choice: ");
            scanf(" %d", &choice);
            clear_stdin();

            switch (choice) {
                case 1:
                    STATE.current_action = OPENING_FILE;
                    break;
                case 2:
                    STATE.current_action = CREATING_NEW_FILE;
                    break;
                case 3:
                    STATE.current_action = SAVING_FILE;
                    break;
                case 4:
                    STATE.current_action = CLOSING_FILE;
                    break;
                case 5:
                    STATE.current_menu = FILE_OPENED;
                    break;
                default:
                    puts("Invalid choice!");
            }

            break;

        case ACTION_MENU:
            puts("========Action Menu========");
            puts("(1) Display");
            puts("(2) Output all sprites info");
            puts("(3) Output sprite info");
            puts("(4) Add Sprite");
            puts("(5) Edit Menu");
            puts("(6) Delete Sprite");

            puts("(7) return back");

            printf("Enter your choice: ");
            scanf(" %d", &choice);
            clear_stdin();

            switch (choice) {
                case 1:
                    STATE.current_action = DISPLAYING;
                    break;
                case 2:
                    STATE.current_action = OUTPUTTING_ALL_SPRITES_INFO;
                    break;
                case 3:
                    STATE.current_action = OUTPUTTING_SPRITE_INFO;
                    break;
                case 4:
                    STATE.current_action = ADDING_SPRITE;
                    break;
                case 5:
                    STATE.current_menu = EDIT_MENU;
                    break;
                case 6:
                    STATE.current_action = DELETING_SPRITE;
                    break;
                case 7:
                    STATE.current_menu = FILE_OPENED;
                    break;
                default:
                    puts("Invalid choice!");
            }

            break;

        case EDIT_MENU:
            if (edit_sprite_number == -1) {
                output_sprites_to_stdout(SPRITES);

                printf("Enter sprite index: ");
                scanf(" %d", &edit_sprite_number);
                clear_stdin();

                if (edit_sprite_number < 0 || edit_sprite_number >= sprites_len(SPRITES)) {
                    printf("Invalid sprite index!\n");
                    edit_sprite_number = -1;
                    return;
                }

                puts("");
            }

            edit_sprite_in_sprites();

            break;

        default:
            puts("Invalid state!");
            if (SPRITES != NULL && SPRITES[0] != NULL && OPENED_FILE_NAME[0] != '\0') {
                save_file();
                STATE.current_action = EXITING_PROGRAM;
            }

            STATE.current_menu = NO_OPENED_FILE;
    }
}

static void action() {
    switch (STATE.current_action) {
        case NO_ACTION:
            break;

        case OPENING_FILE:
            if (SPRITES != NULL && SPRITES[0] != NULL && OPENED_FILE_NAME[0] != '\0') {
                save_file();
                close_file();
                puts("");
            }

            puts("Opening file...");

            printf("Enter File Name: ");
            scanf(" %s", OPENED_FILE_NAME);
            clear_stdin();

            bool is_file_successfully_opened = false;
            SPRITES = input_sprites_from_file(OPENED_FILE_NAME, &is_file_successfully_opened);
            if (!is_file_successfully_opened) {
                STATE.current_menu = NO_OPENED_FILE;
                return;
            }

            STATE.current_menu = FILE_OPENED;
            puts("File successfully opened!");

            break;

        case CREATING_NEW_FILE:
            if (SPRITES != NULL && SPRITES[0] != NULL && OPENED_FILE_NAME[0] != '\0') {
                save_file();
                close_file();
                puts("");
            }

            puts("Creating file...");

            printf("Enter File Name: ");
            scanf(" %s", OPENED_FILE_NAME);
            clear_stdin();

            FILE *file = fopen(OPENED_FILE_NAME, "w");
            if (file == NULL) {
                puts("Couldn't create file!");
                STATE.current_menu = NO_OPENED_FILE;
                return;
            }
            fclose(file);

            puts("File created!");
            STATE.current_menu = FILE_OPENED;

            break;

        case EXITING_PROGRAM:
            /*if (SPRITES != NULL && SPRITES[0] != NULL && OPENED_FILE_NAME[0] != '\0') {
                save_file();
            }*/

            puts("Exiting program...");

            close_file();

            delete_screen(&SCREEN);
            puts("Screen deleted!");

            free(OPENED_FILE_NAME);
            puts("OPENED_FILE_NAME name deleted!");

            break;

        case SAVING_FILE:

            save_file();

            STATE.current_menu = FILE_MENU;

            break;

        case CLOSING_FILE:
            save_file();

            close_file();

            STATE.current_menu = NO_OPENED_FILE;

            break;

        case DISPLAYING:
            puts("Displaying...");

            char ans;
            printf("Test mode? (y/n): ");
            scanf(" %c", &ans);

            bool test_mode = (ans == 'y');

            display_sprites(test_mode);

            STATE.current_menu = ACTION_MENU;

            break;

        case OUTPUTTING_ALL_SPRITES_INFO:
            puts("Outputting all sprites info...");

            output_sprites_to_stdout(SPRITES);

            STATE.current_menu = ACTION_MENU;

            break;

        case OUTPUTTING_SPRITE_INFO:
            puts("Outputting sprite info...");

            int sprite_index_for_output;
            printf("Enter sprite index: ");
            scanf(" %d", &sprite_index_for_output);
            clear_stdin();

            if (sprite_index_for_output < 0 || sprite_index_for_output >= sprites_len(SPRITES)) {
                puts("Invalid sprite index!");
            }
            else {
                output_sprite(SPRITES[sprite_index_for_output], stdout);
            }

            STATE.current_menu = ACTION_MENU;

            break;

        case ADDING_SPRITE:
            puts("Adding sprite...");

            Sprite *sprite = malloc(sizeof(Sprite));
            input_sprite_from_stdin(sprite);

            add_sprite_to_sprites(sprite);

            STATE.current_menu = ACTION_MENU;

            break;

        case DELETING_SPRITE:
            puts("Deleting sprite...");

            output_sprites_to_stdout(SPRITES);

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

            STATE.current_menu = ACTION_MENU;
            break;

        default:
            puts("Invalid state!");
            STATE.current_menu = NO_OPENED_FILE;
    }
}

static void main_loop() {
    while (true) {
        puts("");
        menu();
        puts("");

        action();
        if (STATE.current_action == EXITING_PROGRAM) {
            break;
        }

        STATE.current_action = NO_ACTION;
    }
}

void start() {
    init();

    main_loop();
}