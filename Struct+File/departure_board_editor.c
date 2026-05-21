//
// Created by niki on 5/17/26.
//

#include "departure_board_editor.h"

#include <stdlib.h>
#include <string.h>

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

    //
    strcpy(OPENED_FILE_NAME, "sprites.txt");
    bool is_file_successfully_opened = false;
    SPRITES = input_sprites_from_file(OPENED_FILE_NAME, &is_file_successfully_opened);
    STATE = FILE_OPENED;
    //
}

void save_file() {
    puts("Saving file...");

    FILE *fp = fopen(OPENED_FILE_NAME, "w");
    output_sprites(SPRITES, fp);
    fclose(fp);

    puts("File saved!");
}

static void main_loop() {
    bool exit_program = false;
    while (exit_program == false) {
        puts("");
        switch (STATE) {
            //MENUS
            case NO_OPENED_FILE:
                puts("(1) Open Board");
                puts("(2) Create new Board");
                puts("(3) Exit");

                printf("Enter your choice: ");
                scanf(" %d", &STATE);
                clear_stdin();

                if (STATE < 1 || STATE > 3) {
                    puts("Invalid choice!");
                    STATE = NO_OPENED_FILE;
                    continue;
                }

                break;

            case FILE_OPENED:
                puts("(-3) File");
                puts("(-4) Actions");
                puts("(3) Exit");

                printf("Enter your choice: ");
                scanf(" %d", &STATE);
                clear_stdin();

                if (STATE != -3 && STATE != -4 && STATE != 3) {
                    puts("Invalid choice!");
                    STATE = FILE_OPENED;
                    continue;
                }

                break;

            case IN_FILE_MENU:
                puts("(1) Open Board");
                puts("(2) Create new Board");

                puts("(4) Save Changes");
                puts("(5) Close Board");

                puts("(-2) return back");

                printf("Enter your choice: ");
                scanf(" %d", &STATE);
                clear_stdin();

                if (STATE != 1 && STATE != 2 && STATE != 4 && STATE != 5 && STATE != -2) {
                    puts("Invalid choice!");
                    STATE = IN_FILE_MENU;
                    continue;
                }

                break;

            case IN_ACTION_MENU:
                puts("(6) Display");
                puts("(7) Output all sprites info");
                puts("(8) Output sprite info");
                puts("(9) Add Sprite");
                puts("(10) Edit Sprite");       //also menu
                puts("(11) Delete Sprite");

                puts("(-2) return back");

                printf("Enter your choice: ");
                scanf(" %d", &STATE);
                clear_stdin();

                if (STATE != -2 && (STATE < 6 || STATE > 11)) {
                    puts("Invalid choice!");
                    STATE = IN_ACTION_MENU;
                    continue;
                }

                break;

            case EDITING_SPRITE:
                puts("Editing sprite...");

                output_sprites_to_stdout(SPRITES);

                int sprite_index_for_edit;
                printf("Enter sprite index: ");
                scanf(" %d", &sprite_index_for_edit);
                clear_stdin();

                edit_sprite_in_sprites(sprite_index_for_edit);  //!!!! to make !!!!

                STATE = IN_ACTION_MENU;

                break;

            //NOT A MENUS
            case OPENING_FILE:
                if (SPRITES != NULL && SPRITES[0] != NULL && OPENED_FILE_NAME[0] != '\0') {
                    save_file();
                }

                puts("Opening file...");

                printf("Enter File Name: ");
                scanf(" %s", OPENED_FILE_NAME);
                clear_stdin();

                bool is_file_successfully_opened = false;
                SPRITES = input_sprites_from_file(OPENED_FILE_NAME, &is_file_successfully_opened);
                if (!is_file_successfully_opened) {
                    STATE = NO_OPENED_FILE;
                    continue;
                }

                STATE = FILE_OPENED;
                puts("File successfully opened!");

                break;

            case CREATING_NEW_FILE:
                if (SPRITES != NULL && SPRITES[0] != NULL && OPENED_FILE_NAME[0] != '\0') {
                    save_file();
                }

                puts("Creating file...");

                printf("Enter File Name: ");
                scanf(" %s", OPENED_FILE_NAME);
                clear_stdin();

                FILE *file = fopen(OPENED_FILE_NAME, "w");
                if (file == NULL) {
                    puts("Couldn't create file!");
                    STATE = NO_OPENED_FILE;
                    continue;
                }
                fclose(file);

                puts("File created!");
                STATE = FILE_OPENED;

                break;

            case EXITING_PROGRAM:
                /*if (SPRITES != NULL && SPRITES[0] != NULL && OPENED_FILE_NAME[0] != '\0') {
                    save_file();
                }*/

                puts("Exiting program...");

                if (SPRITES != NULL) {
                    free_sprites_array(SPRITES);
                }

                delete_screen(&SCREEN);

                free(OPENED_FILE_NAME);

                exit_program = true;

                break;

            case SAVING_FILE:

                save_file();

                STATE = IN_FILE_MENU;

                break;

            case CLOSING_FILE:
                puts("Closing file...");

                save_file();

                free_sprites_array(SPRITES);
                SPRITES[0] = NULL;

                OPENED_FILE_NAME[0] = '\0';

                STATE = NO_OPENED_FILE;

                break;

            case DISPLAYING:
                puts("Displaying...");

                char ans;
                printf("Test mode? (y/n): ");
                scanf(" %c", &ans);

                bool test_mode = (ans == 'y');

                display_sprites(test_mode);

                STATE = IN_ACTION_MENU;

                break;

            case OUTPUTTING_ALL_SPRITES_INFO:
                puts("Outputting all sprites info...");

                output_sprites_to_stdout(SPRITES);

                STATE = IN_ACTION_MENU;

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

                STATE = IN_ACTION_MENU;

                break;

            case ADDING_SPRITE:
                puts("Adding sprite...");

                Sprite *sprite = malloc(sizeof(Sprite));
                input_sprite_from_stdin(sprite);

                add_sprite_to_sprites(sprite);

                STATE = IN_ACTION_MENU;

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

                STATE = IN_ACTION_MENU;
                break;

            default:
                puts("Invalid state!");
                STATE = NO_OPENED_FILE;
        }
    }
}

void start() {
    init();

    main_loop();
}