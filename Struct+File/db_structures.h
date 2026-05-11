//db - departure board

#ifndef STRUCT_FILE_DB_STRUCTURES_H
#define STRUCT_FILE_DB_STRUCTURES_H

#define MAX_STRING_SIZE 100
//////////////////////////// Text ////////////////////////////
struct Text{
    char content[MAX_STRING_SIZE];
};

//////////////////////////// Line ////////////////////////////
typedef enum{
    RIGHT = 1,
    DOWN
} DirectionEnum;

struct Line {
    char character;         //can be only visible char. ASCII code > 32
    int length;
    DirectionEnum direction;
};

//////////////////////////// ScheduleSlot ////////////////////////////
typedef enum {
    WAITING = 1,
    IN_PROGRESS,
    CANCELLED
} SlotStatus;

typedef struct{
    int hour;
    int minute;
} TimeType;

typedef struct Slot {
    int trip_number;
    int station_number;

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
    struct Slot slot;
} DetailsType;

//////////////////////// Sprite ////////////////////////////
typedef struct {
    char name[MAX_STRING_SIZE];
    int x;
    int y;

    TypeOfSprite type;
    DetailsType details;        //maybe make to pointer, so slots will represent a fixed place. And info(slots details) will be changed?
} Sprite;

//////////////////////////// Screen ////////////////////////////
typedef struct{
    char **buffer;
    int width;
    int height;
} ScreenBuffer;

#endif //STRUCT_FILE_DB_STRUCTURES_H