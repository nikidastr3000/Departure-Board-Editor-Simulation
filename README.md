# Departure Board Editor

A C-based application for creating and managing departure boards using "Sprites". This tool allows users to define text, lines, and schedule slots, which can then be displayed or saved to a file.

## Features

- **Sprite Management:** Add, edit, delete, and view information for different types of sprites.
- **Sprite Types:**
    - **TEXT:** Simple text labels.
    - **LINE:** Horizontal or vertical lines.
    - **SLOT:** Schedule entries including trip number, station, status, and departure times (scheduled/estimated).
- **File Persistence:** Save and load your departure board configurations to/from text files.
- **Interactive Menu:** A state-driven command-line interface for easy navigation.
- **Configurable Display:** Customize screen dimensions and background characters via a configuration file.

## Project Structure

```
/
├── main.c                      # Entry point
├── departure_board_editor.c/h  # Core logic and state machine
├── globals_and_structures.h    # Data structures and global variables
├── CMakeLists.txt              # Build configuration
└── functions/                  # Modular utilities
    ├── include/                # Header files for functions
    └── src/                    # Implementation files
```

## Configuration

The application requires a `config.txt` file in the execution directory. It should contain the following fields:

- `MAX_STRING_SIZE:` Maximum length for names and text content.
- `SLOT_MARGIN:` Margin for schedule slots.
- `SCREEN_WIDTH:` Width of the display board.
- `SCREEN_HEIGHT:` Height of the display board.
- `SCREEN_BG_CHAR:` ASCII code for the background character.

## Building and Running

### Prerequisites
- CMake
- A C compiler (e.g., GCC or Clang)

### Build Instructions
1. Create a build directory: `mkdir build && cd build`
2. Run CMake: `cmake ..`
3. Build the project: `make`

### Running
Ensure `config.txt` is present in your build or execution directory, then run:
```bash
./Struct_File
```

## Usage

1. **Start Mode:** Choose to open an existing file or create a new one.
2. **Edit Mode:** Once a file is active, you can:
    - Add, edit, or delete sprites.
    - Display the current board.
    - Output sprite information to the console.
    - Save changes or close the current file.
3. **Exit:** Exit the program at any time from the main menu.
