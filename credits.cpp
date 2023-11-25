#include "raylib.h"
#include "credits.h"
#include "main.h"

static Texture2D Dedication[2];
static float DedicationWidth[2];
static float DedicationHeight[2];
static float CreditsAlpha = 0;
static int DedicationNum = 0;

void InitCredits(int screenWidth, int screenHeight)
{
    // Credits resources
    Image img[2];
    img[0] = LoadImage("../resources/maya.png");
    img[1] = LoadImage("../resources/m13.png");
    Dedication[0] = LoadTextureFromImage(img[0]);
    Dedication[1] = LoadTextureFromImage(img[1]);
    DedicationWidth[0] = img[0].width;
    DedicationHeight[0] = img[0].height;
    DedicationWidth[1] = img[1].width;
    DedicationHeight[1] = img[1].height;
    UnloadImage(img[0]);
    UnloadImage(img[1]);
}

GameState ProcessCredits(GameState inState, float dt)
{
    static float CreditsTime = 0;
    CreditsTime += dt;
    if (CreditsTime < 2)
        CreditsAlpha = CreditsTime / 2;
    else if (CreditsTime < 5);
    else if (CreditsTime < 7)
        CreditsAlpha = (7 - CreditsTime) / 2;
    else if (CreditsTime < 9)
    {
        DedicationNum = 1;
        CreditsAlpha = (CreditsTime - 7) / 2;
    }
    else if (CreditsTime > 14)
    {
        CreditsTime = 0;
        DedicationNum = 0;
        return GAME_ON;
    }
    return inState;
}

void DrawCredits(const int screenWidth, const int screenHeight)
{
    BeginDrawing();

    ClearBackground(BLACK);
    if (DedicationNum == 0)
    {
        DrawTexture(Dedication[0], screenWidth / 2 - DedicationWidth[0], screenHeight / 2 - DedicationHeight[0] / 2, Fade(WHITE, CreditsAlpha));
        DrawText("Dedicated to:", screenWidth / 2 + 20, screenHeight / 2 - 30, 40, Fade(RAYWHITE, CreditsAlpha));
        DrawText("Maya Zapunidi", screenWidth / 2 + 20, screenHeight / 2 + 30, 40, Fade(RAYWHITE, CreditsAlpha));
    }
    else if (DedicationNum == 1)
    {
        DrawTexture(Dedication[1], screenWidth / 2 - DedicationWidth[1], screenHeight / 2 - DedicationHeight[1] / 2, Fade(WHITE, CreditsAlpha));
        DrawText("Happy birthday, Maya!", screenWidth / 2 + 20, screenHeight / 2, 40, Fade(RAYWHITE, CreditsAlpha));
    }

    EndDrawing();
}

void UnloadCredits(void)
{
    UnloadTexture(Dedication[0]);
    UnloadTexture(Dedication[1]);
}