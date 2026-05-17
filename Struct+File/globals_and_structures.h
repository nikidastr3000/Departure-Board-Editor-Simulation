//db - departure board

#ifndef STRUCT_FILE_DB_STRUCTURES_H
#define STRUCT_FILE_DB_STRUCTURES_H

//////////////////////////// Text ////////////////////////////
struct Text{
    char *content;      //all chars are allowed (from 0 to 127)
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
    char *name;
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

//////////////////////////// MENU ////////////////////////////
typedef enum {
    IN_START_MODE=-1,
    IN_EDIT_MODE=0,

    //possible from both start and edit mode
    OPENING_FILE = 1,
    CREATING_FILE,
    //RESTORING_PREVIOUS_SESSION,
    EXITING_PROGRAM,

    //possible only from edit mode
    SAVING_FILE,
    CLOSING_FILE,

    DISPLAYING,
    ADDING_SPRITE,
    EDITING_SPRITE,
    DELETING_SPRITE,
} StateEnum;


//GLOBALS
extern int MAX_STRING_SIZE;

extern int SLOT_MARGIN;

extern ScreenType SCREEN;

extern StateEnum state;

extern char *opened_file_name;

extern Sprite **sprites;

#endif //STRUCT_FILE_DB_STRUCTURES_H