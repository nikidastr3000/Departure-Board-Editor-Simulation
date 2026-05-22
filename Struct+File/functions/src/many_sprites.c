//
// Created by niki on 5/13/26.
//

#include "many_sprites.h"

#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "help_functions.h"
#include "input_output.h"

bool add_sprite_to_sprites(const Sprite *sprite) {
    int len = sprites_len(SPRITES);

    Sprite **temp = realloc(SPRITES, (len + 2) * sizeof(Sprite *));     //sprites[size+1] will be the last sprite (=NULL)
    if (temp == NULL) {
        perror("realloc");
        puts("Error! Sprite not added!");
        /*FILE *fp = fopen("last_usage.txt", "w");
        output_sprites(SPRITES, fp);
        fclose(fp);*/
        return false;
    }
    SPRITES = temp;

    SPRITES[len] = (Sprite *)sprite;
    SPRITES[len + 1] = NULL;

    printf("Sprite \"%s\" added!\n", sprite->name);
    return true;
}

bool edit_sprite_in_sprites() {
    bool success = false;

    puts("==========Edit Menu==========");
    printf("Editing sprite number: %d", edit_sprite_number);

    output_sprite(SPRITES[edit_sprite_number], stdout);

    puts("(1) Change the hole sprite");
    puts("(2) Edit name");
    puts("(3) Edit X coordinate");
    puts("(4) Edit Y coordinate");
    puts("(5) Edit sprite type");

    switch (SPRITES[edit_sprite_number]->type) {
        case TEXT:
            success = edit_text();
            break;
        case LINE:
            success = edit_line();
            break;
        case SLOT:
            success = edit_slot();
            break;
        default:
            puts("Invalid sprite type!");
            return false;
    }

    if (edit_sprite_number < -1 || edit_sprite_number >= sprites_len(SPRITES)) {
        puts("Invalid sprite index!");
        edit_sprite_number = -1;
        return false;
    }

    return success;
}

bool edit_text() {
    puts("(6) Edit content");
    puts("(7) Pick other sprite");
    puts("(8) return back");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clear_stdin();

    Sprite *sprite = SPRITES[edit_sprite_number];
    switch (choice) {
        case 1:
            free_sprite(sprite);
            SPRITES[edit_sprite_number] = malloc(sizeof(Sprite));
            input_sprite_from_stdin(SPRITES[edit_sprite_number]);
            break;
        case 2:
            printf("New sprite name: ");
            fgets(sprite->name, MAX_STRING_SIZE, stdin);
            remove_newline(sprite->name);
            break;
        case 3:
            printf("New X coord: ");
            scanf(" %d", &sprite->x);
            clear_stdin();
            break;
        case 4:
            printf("New Y coord: ");
            scanf(" %d", &sprite->y);
            clear_stdin();
            break;
        case 5:
            puts("Types: (1)TEXT / (2)LINE / (3)SLOT");
            printf("New sprite type: ");
            scanf(" %d", &sprite->type);
            clear_stdin();
            break;
        case 6:
            printf("New content: ");
            fgets(sprite->details.text.content, MAX_STRING_SIZE, stdin);
            remove_newline(sprite->details.text.content);
            break;
        case 7:
            puts("");
            output_sprites_to_stdout(SPRITES);
            printf("Enter new sprite index: ");
            scanf(" %d", &edit_sprite_number);
            clear_stdin();
            break;
        case 8:
            edit_sprite_number = -1;
            STATE.current_menu = ACTION_MENU;
            break;
        default:
            puts("Invalid choice!");
            return false;
    }

    return true;
}
bool edit_line() {
    puts("(6) Edit char");
    puts("(7) Edit length");
    puts("(8) Edit direction");
    puts("(9) Pick other sprite");
    puts("(10) return back");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clear_stdin();

    Sprite *sprite = SPRITES[edit_sprite_number];
    switch (choice) {
        case 1:
            free_sprite(sprite);
            SPRITES[edit_sprite_number] = malloc(sizeof(Sprite));
            input_sprite_from_stdin(SPRITES[edit_sprite_number]);
            break;
        case 2:
            printf("New sprite name: ");
            fgets(sprite->name, MAX_STRING_SIZE, stdin);
            remove_newline(sprite->name);
            break;
        case 3:
            printf("New X coord: ");
            scanf(" %d", &sprite->x);
            clear_stdin();
            break;
        case 4:
            printf("New Y coord: ");
            scanf(" %d", &sprite->y);
            clear_stdin();
            break;
        case 5:
            puts("Types: (1)TEXT / (2)LINE / (3)SLOT");
            printf("New sprite type: ");
            scanf(" %d", &sprite->type);
            clear_stdin();
            break;
        case 6:
            printf("New char: ");
            sprite->details.line.character = (char)getchar();
            clear_stdin();
            break;
        case 7:
            printf("New length: ");
            scanf(" %d", &sprite->details.line.length);
            clear_stdin();
            break;
        case 8:
            puts("Directions: (1)RIGHT / (2)DOWN");
            printf("New direction: ");
            scanf(" %d", &sprite->details.line.direction);
            clear_stdin();
            break;
        case 9:
            puts("");
            output_sprites_to_stdout(SPRITES);
            printf("Enter new sprite index: ");
            scanf(" %d", &edit_sprite_number);
            clear_stdin();
            break;
        case 10:
            edit_sprite_number = -1;
            STATE.current_menu = ACTION_MENU;
            break;
        default:
            puts("Invalid choice!");
            return false;
    }

    return true;
}
bool edit_slot() {
    puts("(6) Edit station number");
    puts("(7) Edit scheduled departure time");
    puts("(8) Edit estimated departure time");
    puts("(9) Edit trip number");
    puts("(10) Edit status");
    puts("(11) Pick other sprite");
    puts("(12) return back");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clear_stdin();

    Sprite *sprite = SPRITES[edit_sprite_number];
    switch (choice) {
        case 1:
            free_sprite(sprite);
            SPRITES[edit_sprite_number] = malloc(sizeof(Sprite));
            input_sprite_from_stdin(SPRITES[edit_sprite_number]);
            break;
        case 2:
            printf("New sprite name: ");
            fgets(sprite->name, MAX_STRING_SIZE, stdin);
            remove_newline(sprite->name);
            break;
        case 3:
            printf("New X coord: ");
            scanf(" %d", &sprite->x);
            clear_stdin();
            break;
        case 4:
            printf("New Y coord: ");
            scanf(" %d", &sprite->y);
            clear_stdin();
            break;
        case 5:
            puts("Types: (1)TEXT / (2)LINE / (3)SLOT");
            printf("New sprite type: ");
            scanf(" %d", &sprite->type);
            clear_stdin();
            break;
        case 6:
            printf("New station number: ");
            scanf(" %d", &sprite->details.slot.station_number);
            clear_stdin();
            break;
        case 7:
            printf("New scheduled departure time(hh:mm): ");
            input_time_from_stdin(&sprite->details.slot.scheduled_departure);
            break;
        case 8:
            printf("New estimated departure time(hh:mm): ");
            input_time_from_stdin(&sprite->details.slot.estimated_departure);
            break;
        case 9:
            printf("New trip number: ");
            scanf(" %d", &sprite->details.slot.trip_number);
            clear_stdin();
            break;
        case 10:
            puts("Statuses: (1)WAITING / (2)IN_PROGRESS / (3)CANCELLED");
            printf("New status: ");
            scanf(" %d", &sprite->details.slot.status);
            clear_stdin();
            break;
        case 11:
            puts("");
            output_sprites_to_stdout(SPRITES);
            printf("Enter new sprite index: ");
            scanf(" %d", &edit_sprite_number);
            clear_stdin();
            break;
        case 12:
            edit_sprite_number = -1;
            STATE.current_menu = ACTION_MENU;
            break;
        default:
            puts("Invalid choice!");
            return false;
    }

    return true;
}



bool delete_sprite_from_sprites(int sprite_index) {
    int len = sprites_len(SPRITES);

    if (sprite_index < 0 || sprite_index >= len) {
        printf("Invalid sprite index!\n");
        return false;
    }

    for (int i = sprite_index; i < len - 1; i++) {
        SPRITES[i] = SPRITES[i + 1];
    }
    free_sprite(SPRITES[len - 1]);
    SPRITES[len - 1] = NULL;

    printf("Sprite deleted!\n");
    return true;
}

int sprites_len(Sprite **sprites) {
    if (sprites == NULL) return 0;

    int len = 0;
    for (; sprites[len] != NULL; len++) {}
    return len;
}