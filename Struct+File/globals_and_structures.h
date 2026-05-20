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
    int station_number;         // >= 0

    TimeType scheduled_departure;
    TimeType estimated_departure;

    int trip_number;            // >= 0

    SlotStatus status;
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
    ////////MENUS
    NO_OPENED_FILE = -1,            //the first menu that the user sees
    FILE_OPENED = -2,               //the second menu(after the user opened or created a file
    IN_FILE_MENU = -3,              //the submenu(in second menu) to open/create/save or close a file
    IN_ACTION_MENU = -4,            //the submenu(in second menu) to output/add/edit/delete a sprite or display/output all the sprites
    IN_EDIT_MENU = -5,

    ////////FILES

    //reachable from both 'NO_OPENED_FILE' and 'IN_FILE_MENU'
    OPENING_FILE = 1,
    CREATING_NEW_FILE = 2,
    //RESTORING_PREVIOUS_SESSION,
    EXITING_PROGRAM = 3,

    //reachable only from 'IN_ACTION_MENU'
    SAVING_FILE = 4,            //writes SPRITES to the current file
    CLOSING_FILE = 5,           //clears 'SPRITES' and 'OPENED_FILE_NAME'
    //also saves the file

    ////////SPRITES
    DISPLAYING = 6,
    OUTPUTTING_ALL_SPRITES_INFO = 7,
    OUTPUTTING_SPRITE_INFO = 8,
    ADDING_SPRITE = 9,
    EDITING_SPRITE = 10,
    DELETING_SPRITE = 11,
} StateEnum;


//GLOBALS
extern int MAX_STRING_SIZE;

extern int SLOT_MARGIN;

extern ScreenType SCREEN;

extern StateEnum STATE;

extern char *OPENED_FILE_NAME;

extern Sprite **SPRITES;

#endif //STRUCT_FILE_DB_STRUCTURES_H