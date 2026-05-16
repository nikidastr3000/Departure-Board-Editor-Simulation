//
// Created by niki on 5/13/26.
//

#include "input_output.h"
#include "help_functions.h"

#include <stdlib.h>
#include <string.h>


void input_sprite(Sprite *sprite) {
    puts("Inputting Sprite");

    printf("Sprite_name: ");
    fgets(sprite->name, MAX_STRING_SIZE, stdin);
    remove_newline(sprite->name);       //don't need '\n' at the end

    printf("X_coord: ");
    scanf(" %d", &sprite->x);
    clear_stdin();

    printf("Y_coord: ");
    scanf(" %d", &sprite->y);
    clear_stdin();


    puts("Types: (1)TEXT / (2)LINE / (3)SLOT");
    printf("Type: ");
    scanf(" %d", &sprite->type);
    clear_stdin();

    input_details(&sprite->details, sprite->type);

    puts("Input successfully ended!\n");
}
void input_details(DetailsType *details, const TypeOfSprite type) {
    switch (type) {
        case TEXT:
            printf("Text: ");
            fgets(details->text.content, MAX_STRING_SIZE, stdin);
            remove_newline(details->text.content);
            break;

        case LINE:
            printf("Line_char: ");
            details->line.character = (char)getchar();
            if (details->line.character <= 32 || details->line.character >= 127) {
                puts("Invalid character for Line!");
                exit(1);
            }
            clear_stdin();

            printf("Length: ");
            scanf(" %d", &details->line.length);
            clear_stdin();

            puts("Directions: (1)RIGHT / (2)DOWN");
            printf("Direction: ");
            scanf(" %d", &details->line.direction);
            clear_stdin();
            break;

        case SLOT:
            printf("Trip_number: ");
            scanf(" %d", &details->slot.trip_number);
            clear_stdin();

            printf("Station_number: ");
            scanf(" %d", &details->slot.station_number);
            clear_stdin();

            puts("Statuses: (1)WAITING / (2)IN_PROGRESS / (3)CANCELLED");
            printf("Status: ");
            scanf(" %d", &details->slot.status);
            clear_stdin();

            printf("Scheduled_departure(hh:mm): ");
            input_time(&details->slot.scheduled_departure);
            printf("Estimated_departure(hh:mm): ");
            input_time(&details->slot.estimated_departure);

            break;

        default:
            puts("Invalid Sprite Type!");
            exit(1);
    }
}
void input_time(TimeType *time) {
    char str_time[6];                   //6th char for '\0'
    if (scanf(" %5s", str_time) != 1) {
        puts("Invalid input format!");
        exit(1);
    }
    clear_stdin();

    if (sscanf(str_time, "%d:%d", &time->hours, &time->minutes) != 2 || 
        time->hours > 23 || time->minutes > 59) {
        puts("Invalid time!");
        exit(1);
    }
}


Sprite **input_sprites_from_file(const char *filename) {
    printf("Inputting Sprites from %s\n", filename);

    FILE *source = fopen(filename, "r");
    if (source == NULL) {
        puts("Couldn't open file!");
        exit(1);
    }

    int array_capacity = 4;
    int array_size = 0;
    Sprite **sprites = malloc(array_capacity * sizeof(Sprite *));


    while (1) {
        skip_whitespaces(source);
        int c = fgetc(source);
        if (c == EOF) {
            if (feof(source)) {
                break;
            }

            puts("Couldn't read file!");
            exit(1);
        }
        ungetc(c, source);

        if (array_size >= array_capacity) {
            array_capacity *= 2;
            Sprite **temp = realloc(sprites, array_capacity * sizeof(Sprite *));
            if (!temp) {
                perror("Failed to reallocate sprites");
                exit(1);
            }
            sprites = temp;
        }

        sprites[array_size] = malloc(sizeof(Sprite));
        input_sprite_from_file(sprites[array_size], source);
        array_size++;
    }

    sprites[array_size] = NULL;
    fclose(source);
    puts("Input successfully ended!\n");
    return sprites;
}

void input_sprite_from_file(Sprite *sprite, FILE *source) {
    find_field_in_file(source, "Sprite_name:");
    fgets(sprite->name, MAX_STRING_SIZE, source);
    remove_newline(sprite->name);

    find_field_in_file(source, "X_coord:");
    fscanf(source, " %d\n", &sprite->x);

    find_field_in_file(source, "Y_coord:");
    fscanf(source, " %d\n", &sprite->y);

    input_spriteType_from_file(&sprite->type, source);

    input_details_from_file(&sprite->details, sprite->type, source);
}
void input_spriteType_from_file(TypeOfSprite *type, FILE *source) {
    find_field_in_file(source, "Type:");
    char str_type[MAX_STRING_SIZE];
    fscanf(source, " %s", str_type);
    if (strcmp(str_type, "TEXT") == 0) *type = TEXT;
    else if (strcmp(str_type, "LINE") == 0) *type = LINE;
    else if (strcmp(str_type, "SLOT") == 0) *type = SLOT;
    else {
        puts("Couldn't input Sprite from file. Invalid Sprite Type!");
        exit(1);
    }
}
void input_details_from_file(DetailsType *details, TypeOfSprite type, FILE *source) {
    switch (type) {
        case TEXT:
            find_field_in_file(source, "Text:");
            fgets(details->text.content, MAX_STRING_SIZE, source);
            remove_newline(details->text.content);
            break;
        case LINE:
            find_field_in_file(source, "Line_char:");
            details->line.character = (char)fgetc(source);
            find_field_in_file(source, "Length:");
            fscanf(source, " %d", &details->line.length);

            input_direction_from_file(&details->line.direction, source);
            break;
        case SLOT:
            find_field_in_file(source, "Trip_number:");
            fscanf(source, " %d", &details->slot.trip_number);
            find_field_in_file(source, "Station_number:");
            fscanf(source, " %d", &details->slot.station_number);

            input_status_from_file(&details->slot.status, source);

            find_field_in_file(source, "Scheduled_departure(hh:mm):");
            input_time_from_file(&details->slot.scheduled_departure, source);
            find_field_in_file(source, "Estimated_departure(hh:mm):");
            input_time_from_file(&details->slot.estimated_departure, source);

    }
}
void input_direction_from_file(DirectionEnum *direction, FILE *source) {
    find_field_in_file(source, "Direction:");
    char str_dir[MAX_STRING_SIZE];
    fscanf(source, " %s", str_dir);
    if (strcmp(str_dir, "RIGHT") == 0) *direction = RIGHT;
    else if (strcmp(str_dir, "DOWN") == 0) *direction = DOWN;
    else {
        puts("Couldn't input Sprite from file. Invalid Direction!");
        exit(1);
    }
}
void input_status_from_file(SlotStatus *status, FILE *source) {
    find_field_in_file(source, "Status:");
    char str_status[MAX_STRING_SIZE];
    fscanf(source, " %s", str_status);
    if (strcmp(str_status, "WAITING") == 0) *status = WAITING;
    else if (strcmp(str_status, "IN_PROGRESS") == 0) *status = IN_PROGRESS;
    else if (strcmp(str_status, "CANCELLED") == 0) *status = CANCELLED;
    else {
        puts("Couldn't input Sprite from file. Invalid Status!");
    }
}
void input_time_from_file(TimeType *time, FILE *source) {
    char str_time[6];           //6th char for '\0'
    if (fscanf(source, " %5s", str_time) != 1) {
        puts("Failed to read time from file!");
        exit(1);
    }

    if (sscanf(str_time, "%d:%d", &time->hours, &time->minutes) != 2 ||
        time->hours > 23 || time->minutes > 59) {
        puts("Invalid time!");
        exit(1);
    }
}


void output_sprites(Sprite **sprites, FILE *dest) {
    for (int i = 0; sprites[i] != NULL; i++) {
        output_sprite(sprites[i], dest);
    }
}

void output_sprite(const Sprite *sprite, FILE *dest) {
    fseek(dest, 0, SEEK_END);
    fprintf(dest, "\n");

    fprintf(dest, "Sprite_name: %s\n", sprite->name);
    fprintf(dest, "X_coord: %d\n", sprite->x);
    fprintf(dest, "Y_coord: %d\n", sprite->y);

    output_spriteType(&sprite->type, dest);
    output_details(&sprite->details, sprite->type, dest);

    fprintf(dest, "\n");
}
void output_spriteType(const TypeOfSprite *sprite, FILE *dest) {
    fprintf(dest, "Type: ");
    switch (*sprite) {
        case TEXT:
            fprintf(dest, "TEXT\n");
            break;
        case LINE:
            fprintf(dest, "LINE\n");
            break;
        case SLOT:
            fprintf(dest, "SLOT\n");
            break;
    }
}
void output_details(const DetailsType *details, const TypeOfSprite type, FILE *dest) {
    switch (type) {
        case TEXT:
            fprintf(dest, "Text: %s\n", details->text.content);
            break;
        case LINE:
            fprintf(dest, "Line_char: %c\n", details->line.character);
            fprintf(dest, "Length: %d\n", details->line.length);
            output_direction(&details->line.direction, dest);
            break;
        case SLOT:
            fprintf(dest, "Trip_number: %d\n", details->slot.trip_number);
            fprintf(dest, "Station_number: %d\n", details->slot.station_number);
            output_status(&details->slot.status, dest);
            fprintf(dest, "Scheduled_departure(hh:mm): "); output_time(&details->slot.scheduled_departure, dest); fprintf(dest, "\n");
            fprintf(dest, "Estimated_departure(hh:mm): "); output_time(&details->slot.estimated_departure, dest); fprintf(dest, "\n");
            break;
    }
}
void output_direction(const DirectionEnum *direction, FILE *dest) {
    fprintf(dest, "Direction: ");
    switch (*direction) {
        case RIGHT:
            fprintf(dest, "RIGHT\n");
            break;
        case DOWN:
            fprintf(dest, "DOWN\n");
            break;
    }
}
void output_status(const SlotStatus *status, FILE *dest) {
    fprintf(dest, "Status: ");
    switch (*status) {
        case WAITING:
            fprintf(dest, "WAITING\n");
            break;
        case IN_PROGRESS:
            fprintf(dest, "IN_PROGRESS\n");
            break;
        case CANCELLED:
            fprintf(dest, "CANCELLED\n");
            break;
    }
}
void output_time(const TimeType *time, FILE *dest) {
    if (time->hours < 10) fprintf(dest, "0");
    fprintf(dest, "%d:", time->hours);
    if (time->minutes < 10) fprintf(dest, "0");
    fprintf(dest, "%d", time->minutes);
}


//binary
Sprite **input_sprites_binary(const char *file_name) {
    printf("(binary)Inputting Sprites from %s\n", file_name);

    FILE *fp;
    if ((fp = fopen(file_name, "rb+")) == NULL) {       // attempt to open the file
        fprintf(stderr, "Could not open %s\n", file_name);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    const long file_size = ftell(fp);

    if (file_size % sizeof(Sprite) != 0) {
        fprintf(stderr, "File size is not a multiple of Sprite size!\n");
        exit(1);
    }

    const long arr_size = file_size / (long)sizeof(Sprite);
    rewind(fp);


    Sprite temp_sprites[arr_size];
    fread(temp_sprites, sizeof(Sprite), arr_size, fp);

    Sprite **sprites = malloc(sizeof(Sprite *) * (arr_size + 1));
    for (int i = 0; i < arr_size; i++) {
        sprites[i] = malloc(sizeof(Sprite));
        *sprites[i] = temp_sprites[i];
    }
    sprites[arr_size] = NULL;

    fclose(fp);
    puts("Input successfully ended!\n");

    return sprites;
}

void output_sprites_binary(Sprite **sprites, FILE *dest) {
    for (int i = 0; sprites[i] != NULL; i++) {
        output_sprite_binary(sprites[i], dest);
    }
}
void output_sprite_binary(const Sprite *sprite, FILE *dest) {
    fseek(dest, 0, SEEK_END);
    fwrite(sprite, sizeof(Sprite), 1, dest);
}
