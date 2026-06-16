# Departure Board Editor - Project Documentation

## Project Overview
The **Departure Board Editor** is a C-based command-line application designed for creating, managing, and visualizing departure boards. It uses a "Sprite" system to represent various elements on the board, such as text labels, lines, and schedule slots. The application features a state-driven interactive menu and includes robust collision detection to ensure sprites are placed correctly within the board's boundaries.

### Key Technologies
- **Language:** C (C23 standard)
- **Build System:** CMake (Minimum version 4.1)
- **Architecture:** State-driven menu system with a modular logic structure.
- **Persistence:** Text-based file I/O for saving and loading configurations.

### Core Architecture
- `main.c`: Entry point of the application.
- `departure_board_editor.c/h`: Implements the core logic and state machine.
- `globals_and_structures.h`: Defines the central data structures (`Sprite`, `Text`, `Line`, `Slot`, `ScreenType`) and the application's states (`MenuEnum`, `ActionEnum`).
- `functions/`: Contains modular utilities for display logic, I/O operations, and sprite management.

---

## Building and Running

### Prerequisites
- CMake (Version 4.1 or higher)
- A C compiler (GCC, Clang, or MSVC)

### Build Instructions
```bash
mkdir build
cd build
cmake ..
make
```

### Running the Application
The executable `Struct_File` requires a `config.txt` file in the execution directory.
```bash
./Struct_File
```

### Configuration (`config.txt`)
The configuration file must define the following parameters:
- `MAX_STRING_SIZE`: Maximum character limit for names/content.
- `SLOT_MARGIN`: Spacing for schedule slots.
- `SCREEN_WIDTH` & `SCREEN_HEIGHT`: Dimensions of the board.
- `SCREEN_BG_CHAR`: ASCII code for the background filler.

---

## Development Conventions

### Coding Style & Standards
- **Header Guards:** All header files use `#ifndef`/`#define` guards.
- **State Management:** The application uses an explicit state machine defined by `MenuEnum` and `ActionEnum`. Changes to the UI flow should be managed through these enums.
- **Memory Management:** Manual allocation is used throughout (`malloc`/`free`). Ensure any new structures have corresponding cleanup functions (e.g., `free_sprite`).
- **Data Structures:** Sprites use a union-based approach (`DetailsType`) to handle different types (TEXT, LINE, SLOT) polymorphically.
- **Global State:** Core application state and configuration are managed via `extern` variables in `globals_and_structures.h`. Use these with care to maintain predictability.

### Best Practices
- **Validation:** Always validate sprite boundaries and potential collisions using the built-in `check_sprites` logic before rendering.
- **Modularity:** Add new utility functions to the appropriate files within the `functions/` directory rather than polluting the core editor logic.
- **Error Handling:** Functions should return `bool` or error codes to indicate success/failure, especially for I/O operations.
- **I/O Wrappers:** Use the helper functions in `input_output.c` and `help_functions.c` for standard input and file operations to ensure consistent behavior (e.g., newline removal, buffer clearing).

### Testing
While there is no dedicated test framework, the application includes a `test_mode` for the display functions that prints coordinate axes and helps identify alignment issues.
