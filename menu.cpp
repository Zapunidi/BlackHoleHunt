// For some reason the following define must go before raylib and not just raygui
#define _CRT_SECURE_NO_WARNINGS 
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef _CRT_SECURE_NO_WARNINGS

#include "menu.h"

// Black hole hunting: controls initialization
//----------------------------------------------------------------------------------
bool DropdownDifficultyEditMode = false;
int DropdownDifficultyActive = 1;
bool ButtonStartPressed = false;
bool ButtonExitPressed = false;
bool TextBoxCodeEditMode = false;
char TextBoxCodeText[128] = "";

void InitMenu(int screenWidth, int screenHeight)
{
    return;
}

ProgramState ProcessMenu(ProgramState inState, float dt)
{
    if (ButtonStartPressed && TextIsEqual(TextBoxCodeText, "999"))
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
        
        if (DropdownDifficultyEditMode)
            GuiLock();

        GuiPanel((Rectangle){ 104, 144, 176, 208 }, NULL);
        GuiLabel((Rectangle){ 120, 144, 128, 24 }, "Difficulty:");
        GuiLine((Rectangle){ 112, 192, 160, 24 }, NULL);
        ButtonStartPressed = GuiButton((Rectangle){ 120, 216, 144, 24 }, "Start Game"); 
        GuiLine((Rectangle){ 112, 288, 160, 24 }, NULL);
        ButtonExitPressed = GuiButton((Rectangle){ 120, 312, 144, 24 }, "Exit game"); 
        if (GuiTextBox((Rectangle){ 120, 264, 144, 24 }, TextBoxCodeText, 128, TextBoxCodeEditMode))
            TextBoxCodeEditMode = !TextBoxCodeEditMode;
        GuiLabel((Rectangle){ 120, 240, 120, 24 }, "Code:");
        if (GuiDropdownBox((Rectangle){ 120, 168, 144, 24 }, "Easy;Normal;Hard", &DropdownDifficultyActive, DropdownDifficultyEditMode))
            DropdownDifficultyEditMode = !DropdownDifficultyEditMode;
            
        GuiUnlock();

    EndDrawing();
}

void UnloadMenu(void)
{
    return;
}