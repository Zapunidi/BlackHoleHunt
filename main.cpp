/*******************************************************************************************
*   Black hole hunt
*   
*   Originally created as raylib example. Modified to be a game by Zap.
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2016-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "main.h"
#include "menu.h"
#include "game.h"
#include "credits.h"

static GameState state = GAME_MENU;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 900;
    int FPS = 60;

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // NOTE: Try to enable MSAA 4X
    SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(screenWidth, screenHeight, "Black hole hunt");
    InitAudioDevice();                  // Initialize audio device

    Music music = LoadMusicStream("../resources/mini1111.xm");
    Sound soundEat = LoadSound("../resources/eat.wav");
    
    Image icon = LoadImage("../resources/icon.png");
    SetWindowIcon(icon);

    music.looping = true;
    PlayMusicStream(music);

    InitMenu(screenWidth, screenHeight);
    InitGame(screenWidth, screenHeight);
    InitCredits(screenWidth, screenHeight);
    SetSound(soundEat);

    TraceLog(LOG_INFO, "MAIN: Set target FPS to %i", FPS);
    SetTargetFPS(FPS);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateMusicStream(music);      // Update music buffer with new stream data

        float dt = GetFrameTime();

        if (state == GAME_MENU)
            state = ProcessMenu(state, dt);

        else if (state == GAME_ON || state == GAME_WON)
            state = ProcessGame(state, dt);

        else if (state == GAME_CREDITS)
            state = ProcessCredits(state, dt);

        else if (state == GAME_EXIT)
            break;
        
        // Draw
        //----------------------------------------------------------------------------------
        if (state == GAME_MENU)
            DrawMenu(screenWidth, screenHeight);

        else if (state == GAME_ON || state == GAME_WON)
            DrawGame(screenWidth, screenHeight);
        
        else if (state == GAME_CREDITS)
            DrawCredits(screenWidth, screenHeight);
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(music);           // Unload music stream buffers from RAM
    UnloadSound(soundEat);              // Unload music stream buffers from RAM
    UnloadImage(icon);

    UnloadCredits();
    UnloadGame();
    UnloadMenu();

    CloseAudioDevice();                 // Close audio device (music streaming is automatically stopped)

    CloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}