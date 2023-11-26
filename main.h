#ifndef MAIN_H
#define MAIN_H
#include "raylib.h"

enum ProgramState {
    GAME_MENU, // Starting menu
    GAME_ON, // When the game is going on
    GAME_CREDITS, // Showing credits
    GAME_EXIT // Not used for now.
    }; 

extern int DropdownDifficultyActive;

#endif // MAIN_H