#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "menu.h"

// Black hole hunting: controls initialization
//----------------------------------------------------------------------------------
bool DropdownDifficultyEditMode = false;
int DropdownDifficultyActive = 0;
bool ButtonStartPressed = false;
bool ButtonExitPressed = false;

void InitMenu(int screenWidth, int screenHeight)
{
    return;
}

GameState ProcessMenu(GameState inState, float dt)
{
    if (ButtonStartPressed)
    {
        HideCursor();
        return GAME_ON;
    }
    if (ButtonExitPressed)
    {
        return GAME_EXIT;
    }
    return inState;
}

void DrawMenu(const int screenWidth, const int screenHeight)
{
    BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

    // raygui: controls drawing
    //----------------------------------------------------------------------------------
    if (DropdownDifficultyEditMode) GuiLock();

    GuiPanel((Rectangle){ 104, 136, 176, 160 }, NULL);
    GuiLabel((Rectangle){ 128, 136, 128, 24 }, "Difficulty:");
    GuiLine((Rectangle){ 112, 184, 160, 24 }, NULL);
    ButtonStartPressed = GuiButton((Rectangle){ 120, 208, 144, 24 }, "Start Game"); 
    GuiLine((Rectangle){ 112, 232, 160, 24 }, NULL);
    ButtonExitPressed = GuiButton((Rectangle){ 120, 256, 144, 24 }, "Exit game"); 
    if (GuiDropdownBox((Rectangle){ 120, 160, 144, 24 }, "Easy,Normal,Hard", &DropdownDifficultyActive, DropdownDifficultyEditMode)) DropdownDifficultyEditMode = !DropdownDifficultyEditMode;
            
    GuiUnlock();

    EndDrawing();
}

void UnloadMenu(void)
{
    return;
}