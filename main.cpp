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

#include "raylib.h"
#include "main.h"
#include <math.h>

#define CIRCLES_SPEED               300
#define NUMBER_OF_PLAYER_CIRCLES    4
#define INITIAL_PLAYER_RADIUS       6
#define WARP_BOOST_ACTIVE_TIME      1.0f
#define WARP_BOOST_COOLDOWN_TIME    2.0f

static unsigned int planets_number = 0;

int GravityCollision (Player& plr, CircleWave* planets, unsigned int& planets_number, float dt, Sound soundEat);

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

    float difficulty[11] = {0, 3, 4, 5, 5.5, 5.8, 6, 6.2, 6.3, 6.5, 6.7};
    int level = 1;
    GameState state = GAME_ON;

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // NOTE: Try to enable MSAA 4X
    SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(screenWidth, screenHeight, "Black hole hunt");
    HideCursor();
    InitAudioDevice();                  // Initialize audio device

    Music music = LoadMusicStream("../resources/mini1111.xm");
    Sound soundEat = LoadSound("../resources/eat.wav");
    
    Image icon = LoadImage("../resources/icon.png");
    SetWindowIcon(icon);

    // Credits resources
    Image img[2];
    Texture2D Dedication[2];
    float DedicationWidth[2];
    float DedicationHeight[2];
    float CreditsAlpha = 0;
    int DedicationNum = 0;
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
    
    music.looping = true;
    PlayMusicStream(music);
    
    // Create Planets
    CircleWave* planets = InitPlanets(NULL, screenWidth, screenHeight, difficulty[level]);

    // Create Player
    Player plr;
    InitPlayer(plr);

    bool pause = false;

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

        if (IsKeyPressed(KEY_SPACE))
        {
            planets = InitPlanets(planets, screenWidth, screenHeight, difficulty[level]);
            InitPlayer(plr);

            pause = false;
        }

        if (state == GAME_ON)
        {
            if (IsCursorOnScreen())
                plr.position = GetMousePosition();
            else
                ;// plr.position = {-1E5, -1E5};
        }

        if (state == GAME_WON)
        {
            plr.radius *= 1 + dt;
            if (plr.radius >= (screenWidth + screenHeight))
            {
                state = GAME_CREDITS;
                level = 0; // no planets, need to reinitialize
            }
        }
        if (state == GAME_CREDITS)
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
                state = GAME_ON;
            }
        }

        // Player
        if (plr.warp.state == WARP_STATE_ACTIVE)
        {
            plr.warp.activeLeft -= dt;
            if (plr.warp.activeLeft < 0)
            {
                plr.warp.state = WARP_STATE_COOLDOWN;
                plr.warp.activeLeft = WARP_BOOST_ACTIVE_TIME;
            }   
        }
        else if (plr.warp.state == WARP_STATE_COOLDOWN)
        {
            plr.warp.cooldown -= dt;
            if (plr.warp.cooldown < 0)
            {
                plr.warp.cooldown = WARP_BOOST_COOLDOWN_TIME;
                plr.warp.state = WARP_STATE_READY;
            }
                
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            plr.warp.state = WARP_STATE_ACTIVE;
        }

        // Game physics
        if (plr.warp.state != WARP_STATE_ACTIVE)
            if (-1 == GravityCollision(plr, planets, planets_number, dt, soundEat))
                pause = true;

        for (int i = planets_number - 1; (i >= 0) && !pause; i--)
        {
            // Motion and bouncing
            planets[i].position.x += planets[i].v.x * dt;
            planets[i].position.y += planets[i].v.y * dt;

            if (
                ((planets[i].position.y + planets[i].radius >= screenHeight) && (planets[i].v.y > 0)) ||
                ((planets[i].position.y - planets[i].radius <= 0) && (planets[i].v.y < 0))
            )
                planets[i].v.y = -0.5 * planets[i].v.y;
            if (
                ((planets[i].position.x + planets[i].radius >= screenWidth) && (planets[i].v.x > 0)) ||
                ((planets[i].position.x - planets[i].radius <= 0) && (planets[i].v.x < 0))
            )
                planets[i].v.x = -0.5 * planets[i].v.x;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            if (state == GAME_ON || state == GAME_WON)
            {
                ClearBackground(RAYWHITE);

                // Planets
                for (int i = planets_number - 1; i >= 0; i--)
                {
                    DrawCircleV(planets[i].position, planets[i].radius, Fade(planets[i].color, planets[i].alpha));
                }
            
                // Player
                if (plr.warp.state == WARP_STATE_ACTIVE)
                    DrawCircleV(plr.position, plr.radius, Fade(plr.color, 0.3));
                else
                    DrawCircleV(plr.position, plr.radius, plr.color);
                if (plr.warp.state != WARP_STATE_ACTIVE)
                    for (int i = 0; i < NUMBER_OF_PLAYER_CIRCLES; i++)
                    {
                        plr.gravity_lines_shift -= dt * 0.5;
                        if (plr.gravity_lines_shift < 0)
                            plr.gravity_lines_shift += 1;
                        DrawCircleLines(
                            plr.position.x, plr.position.y, 
                            plr.radius * (NUMBER_OF_PLAYER_CIRCLES - i + plr.gravity_lines_shift), 
                            Fade(plr.color, plr.alpha * (i + 1 - plr.gravity_lines_shift) / (NUMBER_OF_PLAYER_CIRCLES + 1))
                            );
                    }

                // Draw help instructions
                DrawText("Press SPACE to restart level", 40, 40, 20, BLACK);
                DrawText(TextFormat("Planets remaining: %02i. Level %i", planets_number, level), 40, 70, 20, BLACK);
            }
            else if (state == GAME_CREDITS)
            {
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
            }
        EndDrawing();
        //----------------------------------------------------------------------------------

        // Winning
        if (planets_number == 0)
        {
            if (level < 10)
            {
                level += 1;
                planets = InitPlanets(planets, screenWidth, screenHeight, difficulty[level]);
                InitPlayer(plr);
            }
            else
                state = GAME_WON;
        }

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(music);          // Unload music stream buffers from RAM
    UnloadSound(soundEat);          // Unload music stream buffers from RAM
    UnloadImage(icon);

    UnloadTexture(Dedication[0]);
    UnloadTexture(Dedication[1]);

    CloseAudioDevice();     // Close audio device (music streaming is automatically stopped)

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void InitPlayer(Player& plr)
{
    plr.radius = INITIAL_PLAYER_RADIUS;
    plr.alpha = 1;
    plr.gravity_lines_shift = 0;
    plr.color = BLACK;
    plr.warp.activeLeft = WARP_BOOST_ACTIVE_TIME;
    plr.warp.cooldown = WARP_BOOST_COOLDOWN_TIME;
    plr.warp.state = WARP_STATE_ACTIVE;
}

CircleWave* InitPlanets(CircleWave* planets, int screenWidth, int screenHeight, float difficulty)
{
    Color colors[13] = { ORANGE, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK,
                         GREEN, SKYBLUE, PURPLE, BEIGE };

    planets_number = 1 + (unsigned int)(expf((float)difficulty / 3) * 2);
    if (planets != NULL)
        planets = (CircleWave*)MemRealloc(planets, planets_number * sizeof(CircleWave));
    else
        planets = (CircleWave*)MemAlloc(planets_number * sizeof(CircleWave));

    for (int i = planets_number - 1; i >= 0; i--)
    {
        planets[i].alpha = 0.8f;
        planets[i].radius = (float)INITIAL_PLAYER_RADIUS / 1.3 * expf((float)i / 5);
        planets[i].position.x = (float)GetRandomValue((int)planets[i].radius, (int)(screenWidth - planets[i].radius));
        planets[i].position.y = (float)GetRandomValue((int)planets[i].radius, (int)(screenHeight - planets[i].radius));
        planets[i].v.x = (float)GetRandomValue(-CIRCLES_SPEED, CIRCLES_SPEED);
        planets[i].v.y = (float)GetRandomValue(-CIRCLES_SPEED, CIRCLES_SPEED);
        planets[i].color = colors[GetRandomValue(0, 12)];
    }
    
    return planets;
}

int GravityCollision (Player& plr, CircleWave* planets, unsigned int& planets_number, float dt, Sound soundEat)
{
    for (int i = planets_number - 1; i >= 0; i--)
    {
        // Gravity
        float dx = plr.position.x - planets[i].position.x;
        float dy = plr.position.y - planets[i].position.y;
        float distance2 = dx * dx + dy * dy;
        float collision = plr.radius + planets[i].radius;
        if (distance2 < collision * collision && !plr.warp.state == WARP_STATE_ACTIVE) // bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2);
        {
            if (plr.radius > planets[i].radius) // Eat
            {
                float growth = sqrtf(plr.radius * plr.radius + planets[i].radius * planets[i].radius) / plr.radius;
                plr.radius *= growth;
                planets[i] = planets[--planets_number];
                PlaySound(soundEat);
                continue;
            }
            else // Die
            {
                plr.color = planets[i].color;
                return -1;
            }
        }
        
        planets[i].v.x += 4000 * dt * plr.radius * dx / distance2;
        planets[i].v.y += 4000 * dt * plr.radius * dy / distance2;
    }
    return 0;
}