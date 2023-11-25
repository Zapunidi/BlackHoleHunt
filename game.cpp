#include "game.h"
#include <math.h>

#define CIRCLES_SPEED               300
#define NUMBER_OF_PLAYER_CIRCLES    4
#define INITIAL_PLAYER_RADIUS       6
#define WARP_FACTOR_TO_RADIUS       4
#define WARP_BOOST_ACTIVE_TIME      1.0f
#define WARP_BOOST_COOLDOWN_TIME    2.0f

#define GLSL_VERSION            330

static float difficulty[11] = {0, 3, 4, 5, 5.5, 5.8, 6, 6.2, 6.3, 6.5, 6.7};
static int level = 1;
static unsigned int planets_number = 0;

Shader warpShader;
RenderTexture2D target;
CircleWave* planets = NULL;
Player plr;
Sound soundEat;

int screenWidthCache, screenHeightCache;
int warpCenterLoc, warpRadiusLoc, warpPlayerLoc;
float warpCenter[2];

int GravityCollision (Player& plr, CircleWave* planets, unsigned int& planets_number, float dt, Sound soundEat);

void SetSound(Sound sound)
{
    soundEat = sound;
}

void InitGame(int screenWidth, int screenHeight)
{
    screenWidthCache = screenWidth;
    screenHeightCache = screenHeight;
    // Create Planets
    planets = InitPlanets(NULL, screenWidth, screenHeight, difficulty[level]);

    // Create Player
    InitPlayer(plr);

    // Shaders
    warpShader = LoadShader(0, TextFormat("../resources/shaders/glsl%i/warp.fs", GLSL_VERSION));
    
    {
        float width = screenWidth, height = screenHeight;
        SetShaderValue(warpShader, GetShaderLocation(warpShader, "renderWidth"), &width, SHADER_UNIFORM_FLOAT);
        SetShaderValue(warpShader, GetShaderLocation(warpShader, "renderHeight"), &height, SHADER_UNIFORM_FLOAT);
    }

    // Get variable (uniform) location on the shader to connect with the program
    // NOTE: If uniform variable could not be found in the shader, function returns -1
    warpCenterLoc = GetShaderLocation(warpShader, "center");
    warpRadiusLoc = GetShaderLocation(warpShader, "radius");
    warpPlayerLoc = GetShaderLocation(warpShader, "playerRadius");
    warpCenter[0] = (float)screenWidth/2;
    warpCenter[1] = (float)screenHeight/2;

    // Create a RenderTexture2D to be used for render to texture
    target = LoadRenderTexture(screenWidth, screenHeight);
}

GameState ProcessGame(GameState inState, float dt)
{
    static bool pause = false;

    if (IsKeyPressed(KEY_SPACE))
    {
        planets = InitPlanets(planets, screenWidthCache, screenHeightCache, difficulty[level]);
        InitPlayer(plr);

        pause = false;
    }
    if (IsCursorOnScreen())
    {
        plr.position = GetMousePosition();
        warpCenter[0] = plr.position.x;
        warpCenter[1] = (screenHeightCache - plr.position.y);
        SetShaderValue(warpShader, warpCenterLoc, warpCenter, SHADER_UNIFORM_VEC2);
        float warpRadius = plr.radius * WARP_FACTOR_TO_RADIUS;
        SetShaderValue(warpShader, warpPlayerLoc, &plr.radius, SHADER_UNIFORM_FLOAT);
        SetShaderValue(warpShader, warpRadiusLoc, &warpRadius, SHADER_UNIFORM_FLOAT);
    }
    else
        ;// plr.position = {-1E5, -1E5};
    if (inState == GAME_WON)
    {
        plr.radius *= 1 + dt;
        if (plr.radius >= (screenWidthCache + screenHeightCache))
        {
            level = 0; // no planets, need to reinitialize
            return GAME_CREDITS;
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
    if (plr.warp.state != WARP_STATE_ACTIVE && ! pause)
        if (-1 == GravityCollision(plr, planets, planets_number, dt, soundEat))
            pause = true;

    for (int i = planets_number - 1; (i >= 0) && !pause; i--)
    {
        // Motion and bouncing
        planets[i].position.x += planets[i].v.x * dt;
        planets[i].position.y += planets[i].v.y * dt;
        if (
            ((planets[i].position.y + planets[i].radius >= screenHeightCache) && (planets[i].v.y > 0)) ||
            ((planets[i].position.y - planets[i].radius <= 0) && (planets[i].v.y < 0))
        )
            planets[i].v.y = -0.5 * planets[i].v.y;
        if (
            ((planets[i].position.x + planets[i].radius >= screenWidthCache) && (planets[i].v.x > 0)) ||
            ((planets[i].position.x - planets[i].radius <= 0) && (planets[i].v.x < 0))
        )
            planets[i].v.x = -0.5 * planets[i].v.x;
    }
    
    // Calculating moving circles visual effect
    plr.gravity_lines_shift -= dt * 2;
    if (plr.gravity_lines_shift < 0)
        plr.gravity_lines_shift += 1;
    
    // Winning
    if (planets_number == 0)
    {
        if (level < 10)
        {
            level += 1;
            planets = InitPlanets(planets, screenWidthCache, screenHeightCache, difficulty[level]);
            InitPlayer(plr);
        }
        else
            return GAME_WON;
    }
    return inState;
}

void DrawGame(const int screenWidth, const int screenHeight)
{
    BeginTextureMode(target);       // Enable drawing to texture
    
        ClearBackground(RAYWHITE);

        // Planets
        for (int i = planets_number - 1; i >= 0; i--)
        {
            DrawCircleV(planets[i].position, planets[i].radius, Fade(planets[i].color, planets[i].alpha));
        }
    
        // Player
        if (plr.warp.state == WARP_STATE_ACTIVE)
        {
            float warpRadius = 1;
            SetShaderValue(warpShader, warpPlayerLoc, &warpRadius, SHADER_UNIFORM_FLOAT);
            warpRadius = WARP_FACTOR_TO_RADIUS;
            SetShaderValue(warpShader, warpRadiusLoc, &warpRadius, SHADER_UNIFORM_FLOAT);
        }
    EndTextureMode();               // End drawing to texture (now we have a texture available for next passes)
    
    BeginDrawing();
        ClearBackground(RAYWHITE);  // Clear screen background
        BeginShaderMode(warpShader);
            // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
            DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 0, 0 }, WHITE);
        EndShaderMode();

        // Player
        if (plr.warp.state == WARP_STATE_ACTIVE)
            DrawCircleV(plr.position, plr.radius, Fade(plr.color, 0.3));
        else
        {
            DrawCircleV(plr.position, plr.radius, plr.color);
            for (int i = 0; i < NUMBER_OF_PLAYER_CIRCLES; i++)
            {
                DrawCircleLines(
                    plr.position.x, plr.position.y, 
                    plr.radius * (NUMBER_OF_PLAYER_CIRCLES - i + plr.gravity_lines_shift), 
                    Fade(plr.color, plr.alpha * (i + 1 - plr.gravity_lines_shift) / (NUMBER_OF_PLAYER_CIRCLES + 1))
                    );
            }
        }
        // Draw help instructions
        DrawText("Press SPACE to restart level", 40, 40, 20, BLACK);
        DrawText(TextFormat("Planets remaining: %02i. Level %i", planets_number, level), 40, 70, 20, BLACK);
    EndDrawing();
}

void UnloadGame(void)
{
    UnloadShader(warpShader);           // Unload shader
    UnloadRenderTexture(target);        // Unload render texture
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
        if (distance2 < collision * collision) // bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2);
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
        
        planets[i].v.x += (1500 + 1500 * DropdownDifficultyActive) * dt * plr.radius * dx / distance2;
        planets[i].v.y += (1500 + 1500 * DropdownDifficultyActive) * dt * plr.radius * dy / distance2;
    }
    return 0;
}