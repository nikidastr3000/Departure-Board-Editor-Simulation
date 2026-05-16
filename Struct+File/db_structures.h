//db - departure board

#ifndef STRUCT_FILE_DB_STRUCTURES_H
#define STRUCT_FILE_DB_STRUCTURES_H

#define MAX_STRING_SIZE 100

#define SLOT_MARGIN 1

//////////////////////////// Text ////////////////////////////
struct Text{
    char content[MAX_STRING_SIZE];      //all chars are allowed (from 0 to 127)
};

//////////////////////////// Line ////////////////////////////
typedef enum{
    RIGHT = 1,
    DOWN
} DirectionEnum;

struct Line {
    char character;         //can be only visible char. ASCII code > 32
    int length;             //can be 0(will be invisible)
    DirectionEnum direction;
};

//////////////////////////// ScheduleSlot ////////////////////////////
typedef enum {
    WAITING = 1,
    IN_PROGRESS,
    CANCELLED
} SlotStatus;

typedef struct{
    int hours;
    int minutes;
} TimeType;

typedef struct Slot {
    int trip_number;            // >= 0
    int station_number;         // >= 0

    SlotStatus status;
    TimeType scheduled_departure;
    TimeType estimated_departure;
} ScheduleSlot;

//////////////////////////// DetailsType ////////////////////////////
typedef enum {
    TEXT = 1,
    LINE,
    SLOT
} TypeOfSprite;

typedef union {
    struct Text text;
    struct Line line;
    struct Slot slot;       //maybe make to pointer, so slots will represent a fixed place. And info(slots details) will be changed?
} DetailsType;

//////////////////////// Sprite ////////////////////////////
typedef struct {
    char name[MAX_STRING_SIZE];
    int x;          //in range from 0 to screen_width - 1
    int y;          //in range from 0 to screen_height - 1

    TypeOfSprite type;
    DetailsType details;
} Sprite;

//////////////////////////// Screen ////////////////////////////
typedef struct{
    char **buffer;
    char bg_char;
    int width;
    int height;
} ScreenType;

#endif //STRUCT_FILE_DB_STRUCTURES_H