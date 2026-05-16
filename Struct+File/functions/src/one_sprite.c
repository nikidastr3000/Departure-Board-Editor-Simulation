//
// Created by niki on 5/13/26.
//

#include "one_sprite.h"

#include <stdlib.h>
#include <string.h>

#include "help_functions.h"

#include "display.h"

int measure_sprite_length(Sprite *sprite) {
    int len = 0;

    switch (sprite->type) {
        case TEXT:
            len = strlen(sprite->details.text.content);
            break;
        case LINE:
            len = sprite->details.line.length;
            break;
        case SLOT:
            char *slot_str = slot_to_string(&sprite->details.slot, ' ');
            len = strlen(slot_str);
            free(slot_str);
            break;
    }

    return len;
}

bool validate_sprite(const Sprite *sprite) {
    if (sprite->name[0] == '\0')
        return false;

    if (sprite->x < 0 || sprite->y < 0)
        return false;

    switch (sprite->type) {
        case TEXT:
            if (sprite->details.text.content[0] == '\0')
                return false;
            break;

        case LINE:
            if (sprite->details.line.character <= 32 || sprite->details.line.character >= 127)
                return false;
            if (sprite->details.line.length < 0)
                return false;
            if (sprite->details.line.direction != RIGHT && sprite->details.line.direction != DOWN)
                return false;
            break;

        case SLOT:
            if (sprite->details.slot.trip_number < 0)
                return false;
            if (sprite->details.slot.station_number < 0)
                return false;
            if (sprite->details.slot.status != WAITING && sprite->details.slot.status != IN_PROGRESS && sprite->details.slot.status != CANCELLED)
                return false;
            if (sprite->details.slot.scheduled_departure.hours < 0 || sprite->details.slot.scheduled_departure.minutes < 0)
                return false;
            if (sprite->details.slot.estimated_departure.hours < 0 || sprite->details.slot.estimated_departure.minutes < 0)
                return false;
            if (sprite->details.slot.scheduled_departure.hours > 23 || sprite->details.slot.scheduled_departure.minutes > 59)
                return false;
            if (sprite->details.slot.estimated_departure.hours > 23 || sprite->details.slot.estimated_departure.minutes > 59)
                return false;
            break;
        default:
            return false;
    }

    return true;
}

//SLOTS_MARGIN*' ' + field_1 + SLOTS_MARGIN*' ' + ' ' + SLOTS_MARGIN*' ' + field_2 + ...
char *slot_to_string(const ScheduleSlot *slot, char bg_char) {
    char *str = malloc(sizeof(char) * 301);     //300 - max size
    int index = 0;

    for (int i = 0; i < SLOT_MARGIN; i++) {
        str[index] = bg_char;
        index++;
    }

    index += sprintf(&str[index], "%d", slot->station_number);      //sprintf returns the number of characters written

    for (int i = 0; i < SLOT_MARGIN; i++) {
        str[index] = bg_char;
        index++;
    }

    str[index] = bg_char;       //space between slot fields
    index++;

    for (int i = 0; i < SLOT_MARGIN; i++) {
        str[index] = bg_char;
        index++;
    }

    index += sprintf(&str[index], "%02d:%02d", slot->scheduled_departure.hours, slot->scheduled_departure.minutes);

    for (int i = 0; i < SLOT_MARGIN; i++) {
        str[index] = bg_char;
        index++;
    }

    str[index] = bg_char;       //space between slot fields
    index++;

    for (int i = 0; i < SLOT_MARGIN; i++) {
        str[index] = bg_char;
        index++;
    }

    index += sprintf(&str[index], "%02d:%02d", slot->estimated_departure.hours, slot->estimated_departure.minutes);

    for (int i = 0; i < SLOT_MARGIN; i++) {
        str[index] = bg_char;
        index++;
    }

    str[index] = bg_char;       //space between slot fields
    index++;

    for (int i = 0; i < SLOT_MARGIN; i++) {
        str[index] = bg_char;
        index++;
    }

    index += sprintf(&str[index], "%d", slot->trip_number);

    for (int i = 0; i < SLOT_MARGIN; i++) {
        str[index] = bg_char;
        index++;
    }

    str[index] = bg_char;       //space between slot fields
    index++;

    for (int i = 0; i < SLOT_MARGIN; i++) {
        str[index] = bg_char;
        index++;
    }

    switch (slot->status) {
        case WAITING:
            index += sprintf(&str[index], "WAITING");
            break;
        case IN_PROGRESS:
            index += sprintf(&str[index], "IN_PROGRESS");
            break;
        case CANCELLED:
            index += sprintf(&str[index], "CANCELLED");
            break;
    }

    for (int i = 0; i < SLOT_MARGIN; i++) {
        str[index] = bg_char;
        index++;
    }

    str[index] = '\0';


    return str;
}

