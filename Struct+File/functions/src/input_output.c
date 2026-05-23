//
// Created by niki on 5/13/26.
//

#include "input_output.h"
#include "help_functions.h"

#include <stdlib.h>
#include <string.h>


void input_sprite_from_stdin(Sprite *sprite) {
    puts("Inputting Sprite");

    sprite->name = malloc(sizeof(char) * MAX_STRING_SIZE);
    if (sprite->name == NULL) {
        perror("Failed to allocate memory for sprite name");
        exit(1);
    }
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

    input_details_from_stdin(&sprite->details, sprite->type);

    puts("Input successfully ended!\n");
}
void input_details_from_stdin(DetailsType *details, const TypeOfSprite type) {
    switch (type) {
        case TEXT:
            details->text.content = malloc(sizeof(char) * MAX_STRING_SIZE);
            if (details->text.content == NULL) {
                perror("Failed to allocate memory for text content");
                exit(1);
            }
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
            printf("Station_number: ");
            scanf(" %d", &details->slot.station_number);
            clear_stdin();

            input_time_from_stdin(&details->slot.scheduled_departure, "Scheduled_departure(hh:mm): ");
            input_time_from_stdin(&details->slot.estimated_departure, "Estimated_departure(hh:mm): ");

            printf("Trip_number: ");
            scanf(" %d", &details->slot.trip_number);
            clear_stdin();

            puts("Statuses: (1)WAITING / (2)IN_PROGRESS / (3)CANCELLED");
            printf("Status: ");
            scanf(" %d", &details->slot.status);
            clear_stdin();

            break;

        default:
            puts("Invalid Sprite Type!");
            exit(1);
    }
}
void input_time_from_stdin(TimeType *time, char *output) {
    printf("%s", output);

    char str_time[6];                   //6th char for '\0'
    if (scanf(" %5s", str_time) != 1) {
        clear_stdin();
        puts("Invalid input format!");
        input_time_from_stdin(time, output);
    }
    clear_stdin();

    if (sscanf(str_time, "%d:%d", &time->hours, &time->minutes) != 2 || 
        time->hours > 23 || time->minutes > 59 || time->hours < 0 || time->minutes < 0) {
        puts("Invalid time!");
        input_time_from_stdin(time, output);
    }
}


Sprite **input_sprites_from_file(const char *filename, bool *is_file_successfully_opened) {
    printf("Inputting Sprites from %s\n", filename);

    FILE *source = fopen(filename, "r");
    if (source == NULL) {
        puts("Couldn't open file!");
        *is_file_successfully_opened = false;
        return NULL;
    }
    else {
        *is_file_successfully_opened = true;
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
        if (!input_sprite_from_file(sprites[array_size], source)) {
            puts("Failed to read sprite from file!");

            if (sprites[array_size] != NULL) {
                free_sprite(sprites[array_size]);
            }
            break;
        }
        array_size++;
    }

    if (array_size == 0) {
        puts("No sprites found!");
        return NULL;
    }

    //allocating memory for the NULL sprite
    if (array_size >= array_capacity) {
        array_capacity *= 2;
        Sprite **temp = realloc(sprites, array_capacity * sizeof(Sprite *));
        if (!temp) {
            perror("Failed to reallocate sprites");
            exit(1);
        }
        sprites = temp;
    }
    sprites[array_size] = NULL;
    fclose(source);
    printf("%d Sprites were successfully read!\n", array_size);
    return sprites;
}

bool input_sprite_from_file(Sprite *sprite, FILE *source) {
    if (!find_field_in_file(source, "Sprite_name:")) {
        sprite->name = NULL;
        return false;
    }
    sprite->name = malloc(sizeof(char) * MAX_STRING_SIZE);
    if (sprite->name == NULL) {
        perror("Failed to allocate memory for sprite name from file");
        exit(1);
    }
    fgets(sprite->name, MAX_STRING_SIZE, source);
    remove_newline(sprite->name);

    if (!find_field_in_file(source, "X_coord:")) {
        return false;
    }
    fscanf(source, " %d\n", &sprite->x);

    if (!find_field_in_file(source, "Y_coord:")) {
        return false;
    }
    fscanf(source, " %d\n", &sprite->y);

    if (!input_spriteType_from_file(&sprite->type, source)) {
        return false;
    }

    if (!input_details_from_file(&sprite->details, sprite->type, source)) {
        return false;
    }

    return true;
}

bool input_spriteType_from_file(TypeOfSprite *type, FILE *source) {
    if (!find_field_in_file(source, "Type:")) {
        return false;
    }
    char str_type[MAX_STRING_SIZE];
    fscanf(source, " %s", str_type);
    if (strcmp(str_type, "TEXT") == 0) *type = TEXT;
    else if (strcmp(str_type, "LINE") == 0) *type = LINE;
    else if (strcmp(str_type, "SLOT") == 0) *type = SLOT;
    else {
        puts("Invalid Sprite Type!");
    }

    return true;
}

bool input_details_from_file(DetailsType *details, TypeOfSprite type, FILE *source) {
    switch (type) {
        case TEXT:
            if (! find_field_in_file(source, "Text:")) {
                details->text.content = NULL;
                return false;
            }
            details->text.content = malloc(sizeof(char) * MAX_STRING_SIZE);
            if (details->text.content == NULL) {
                perror("Failed to allocate memory for text content from file");
                exit(1);
            }
            fgets(details->text.content, MAX_STRING_SIZE, source);
            remove_newline(details->text.content);
            break;
        case LINE:
            if (!find_field_in_file(source, "Line_char:")) {
                return false;
            }
            details->line.character = (char)fgetc(source);
            if (!find_field_in_file(source, "Length:")) {
                return false;
            }
            fscanf(source, " %d", &details->line.length);

            if (!input_direction_from_file(&details->line.direction, source)) {
                return false;
            }
            break;
        case SLOT:
            if (!find_field_in_file(source, "Station_number:")) {
                return false;
            }
            fscanf(source, " %d", &details->slot.station_number);

            if (!find_field_in_file(source, "Scheduled_departure(hh:mm):")) {
                return false;
            }
            input_time_from_file(&details->slot.scheduled_departure, source);
            if (!find_field_in_file(source, "Estimated_departure(hh:mm):")) {
                return false;
            }
            input_time_from_file(&details->slot.estimated_departure, source);

            if (!find_field_in_file(source, "Trip_number:")) {
                return false;
            }
            fscanf(source, " %d", &details->slot.trip_number);

            if (!input_status_from_file(&details->slot.status, source)) {
                return false;
            }
            break;
    }

    return true;
}

bool input_direction_from_file(DirectionEnum *direction, FILE *source) {
    if (!find_field_in_file(source, "Direction:")) {
        return false;
    }
    char str_dir[MAX_STRING_SIZE];
    fscanf(source, " %s", str_dir);
    if (strcmp(str_dir, "RIGHT") == 0) *direction = RIGHT;
    else if (strcmp(str_dir, "DOWN") == 0) *direction = DOWN;
    else {
        puts("Invalid Direction!");
    }

    return true;
}

bool input_status_from_file(SlotStatus *status, FILE *source) {
    if (!find_field_in_file(source, "Status:")) {
        return false;
    }
    char str_status[MAX_STRING_SIZE];
    fscanf(source, " %s", str_status);
    if (strcmp(str_status, "WAITING") == 0) *status = WAITING;
    else if (strcmp(str_status, "IN_PROGRESS") == 0) *status = IN_PROGRESS;
    else if (strcmp(str_status, "CANCELLED") == 0) *status = CANCELLED;
    else {
        puts("Invalid Status!");
    }

    return true;
}

bool input_time_from_file(TimeType *time, FILE *source) {
    char str_time[6];           //6th char for '\0'
    if (fscanf(source, " %5s", str_time) != 1) {
        puts("Failed to read time from file!");
    }

    if (sscanf(str_time, "%d:%d", &time->hours, &time->minutes) != 2 ||
        time->hours > 23 || time->minutes > 59) {
        puts("Invalid time!");
    }

    return true;
}


void output_sprites_to_stdout(Sprite **sprites) {
    if (sprites == NULL || sprites[0] == NULL) {
        puts("Sprites array is empty!");
        return;
    }

    for (int i = 0; sprites[i] != NULL; i++) {
        //1. Title (1,1)
        printf("%d. %s (%d,%d) ", i, sprites[i]->name, sprites[i]->x, sprites[i]->y);

        //details
        switch (sprites[i]->type) {
            case TEXT:
                printf("TEXT: %s", sprites[i]->details.text.content);
                break;
            case LINE:
                printf("LINE: %c %d %s", sprites[i]->details.line.character, sprites[i]->details.line.length, sprites[i]->details.line.direction == RIGHT ? "RIGHT" : "DOWN");
                break;
            case SLOT:
                //SLOT: station_number scheduled_time estimated_time trip_number status
                printf("SLOT: %d %d:%02d %d:%02d %d %s",
                    sprites[i]->details.slot.station_number, sprites[i]->details.slot.scheduled_departure.hours, sprites[i]->details.slot.scheduled_departure.minutes,
                    sprites[i]->details.slot.estimated_departure.hours, sprites[i]->details.slot.estimated_departure.minutes, sprites[i]->details.slot.trip_number,
                    sprites[i]->details.slot.status == WAITING ? "WAITING" : sprites[i]->details.slot.status == IN_PROGRESS ? "IN_PROGRESS" : "CANCELLED");
                break;
        }

        printf("\n");
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
            fprintf(dest, "Station_number: %d\n", details->slot.station_number);
            fprintf(dest, "Scheduled_departure(hh:mm): "); output_time(&details->slot.scheduled_departure, dest); fprintf(dest, "\n");
            fprintf(dest, "Estimated_departure(hh:mm): "); output_time(&details->slot.estimated_departure, dest); fprintf(dest, "\n");
            fprintf(dest, "Trip_number: %d\n", details->slot.trip_number);
            output_status(&details->slot.status, dest);
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


/*//binary
Sprite **input_sprites_binary(const char *file_name) {
    printf("(binary)Inputting Sprites from %s\n", file_name);

    FILE *fp;
    if ((fp = fopen(file_name, "rb+")) == NULL) {       // attempt to open the file
        fprintf(stderr, "Could not open %s\n", file_name);
        exit(1);
    }

    // The binary file format will be:
    // [num_sprites (int)]
    // For each sprite:
    //   [x (int)]
    //   [y (int)]
    //   [type (TypeOfSprite)]
    //   [name_length (int)]
    //   [name (char array)]
    //   If type is TEXT:
    //     [content_length (int)]
    //     [content (char array)]
    //   If type is LINE:
    //     [character (char)]
    //     [length (int)]
    //     [direction (DirectionEnum)]
    //   If type is SLOT:
    //     [trip_number (int)]
    //     [station_number (int)]
    //     [status (SlotStatus)]
    //     [scheduled_departure (TimeType)]
    //     [estimated_departure (TimeType)]

    int num_sprites = 0;
    fread(&num_sprites, sizeof(int), 1, fp);

    Sprite **sprites = malloc(sizeof(Sprite *) * (num_sprites + 1));
    if (sprites == NULL) {
        perror("Failed to allocate memory for sprites array");
        exit(1);
    }

    for (int i = 0; i < num_sprites; i++) {
        sprites[i] = malloc(sizeof(Sprite));
        if (sprites[i] == NULL) {
            perror("Failed to allocate memory for sprite");
            exit(1);
        }

        fread(&sprites[i]->x, sizeof(int), 1, fp);
        fread(&sprites[i]->y, sizeof(int), 1, fp);
        fread(&sprites[i]->type, sizeof(TypeOfSprite), 1, fp);

        int name_length;
        fread(&name_length, sizeof(int), 1, fp);
        sprites[i]->name = malloc(sizeof(char) * (name_length + 1));
        if (sprites[i]->name == NULL) {
            perror("Failed to allocate memory for sprite name");
            exit(1);
        }
        fread(sprites[i]->name, sizeof(char), name_length, fp);
        sprites[i]->name[name_length] = '\0';

        switch (sprites[i]->type) {
            case TEXT: {
                int content_length;
                fread(&content_length, sizeof(int), 1, fp);
                sprites[i]->details.text.content = malloc(sizeof(char) * (content_length + 1));
                if (sprites[i]->details.text.content == NULL) {
                    perror("Failed to allocate memory for text content");
                    exit(1);
                }
                fread(sprites[i]->details.text.content, sizeof(char), content_length, fp);
                sprites[i]->details.text.content[content_length] = '\0';
                break;
            }
            case LINE:
                fread(&sprites[i]->details.line.character, sizeof(char), 1, fp);
                fread(&sprites[i]->details.line.length, sizeof(int), 1, fp);
                fread(&sprites[i]->details.line.direction, sizeof(DirectionEnum), 1, fp);
                break;
            case SLOT:
                fread(&sprites[i]->details.slot.trip_number, sizeof(int), 1, fp);
                fread(&sprites[i]->details.slot.station_number, sizeof(int), 1, fp);
                fread(&sprites[i]->details.slot.status, sizeof(SlotStatus), 1, fp);
                fread(&sprites[i]->details.slot.scheduled_departure, sizeof(TimeType), 1, fp);
                fread(&sprites[i]->details.slot.estimated_departure, sizeof(TimeType), 1, fp);
                break;
        }
    }
    sprites[num_sprites] = NULL;

    fclose(fp);
    puts("Input successfully ended!\n");

    return sprites;
}

void output_sprites_binary(Sprite **sprites, FILE *dest) {
    int num_sprites = 0;
    for (int i = 0; sprites[i] != NULL; i++) {
        num_sprites++;
    }

    fwrite(&num_sprites, sizeof(int), 1, dest);

    for (int i = 0; sprites[i] != NULL; i++) {
        output_sprite_binary(sprites[i], dest);
    }
}
void output_sprite_binary(const Sprite *sprite, FILE *dest) {
    fwrite(&sprite->x, sizeof(int), 1, dest);
    fwrite(&sprite->y, sizeof(int), 1, dest);
    fwrite(&sprite->type, sizeof(TypeOfSprite), 1, dest);

    int name_length = strlen(sprite->name);
    fwrite(&name_length, sizeof(int), 1, dest);
    fwrite(sprite->name, sizeof(char), name_length, dest);

    switch (sprite->type) {
        case TEXT: {
            int content_length = strlen(sprite->details.text.content);
            fwrite(&content_length, sizeof(int), 1, dest);
            fwrite(sprite->details.text.content, sizeof(char), content_length, dest);
            break;
        }
        case LINE:
            fwrite(&sprite->details.line.character, sizeof(char), 1, dest);
            fwrite(&sprite->details.line.length, sizeof(int), 1, dest);
            fwrite(&sprite->details.line.direction, sizeof(DirectionEnum), 1, dest);
            break;
        case SLOT:
            fwrite(&sprite->details.slot.trip_number, sizeof(int), 1, dest);
            fwrite(&sprite->details.slot.station_number, sizeof(int), 1, dest);
            fwrite(&sprite->details.slot.status, sizeof(SlotStatus), 1, dest);
            fwrite(&sprite->details.slot.scheduled_departure, sizeof(TimeType), 1, dest);
            fwrite(&sprite->details.slot.estimated_departure, sizeof(TimeType), 1, dest);
            break;
    }
}*/



void free_sprite(Sprite *sprite) {
    if (sprite == NULL) return;

    if (sprite->name != NULL) {
        free(sprite->name);
        sprite->name = NULL;
    }


    if (sprite->type == TEXT && sprite->details.text.content != NULL) {
        free(sprite->details.text.content);
        sprite->details.text.content = NULL;
    }

    free(sprite);
}
void free_sprites_array(Sprite **sprites) {
    if (sprites == NULL) return;

    for (int i = 0; sprites[i] != NULL; i++) {
        free_sprite(sprites[i]);
    }
    free(sprites);
}