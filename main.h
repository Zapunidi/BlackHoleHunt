#ifndef MAIN_H
#define MAIN_H
#include "raylib.h"

enum GameState {
    GAME_MENU, // Starting menu
    GAME_ON, // When the game is going on
    GAME_WON, // When the game is finished
    GAME_CREDITS, // Showing credits
    GAME_EXIT // Not used for now.
    }; 

#endif // MAIN_H