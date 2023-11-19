#include "raylib.h"

enum GameState {
    GAME_START, // Not used for now. Meant for starting menu
    GAME_ON, // When the game is going on
    GAME_WON, // When the game is finished
    GAME_CREDITS, // Showing credits
    GAME_LOST // Not used for now.
    }; 

enum WarpState {
    WARP_STATE_ACTIVE,
    WARP_STATE_COOLDOWN,
    WARP_STATE_READY
    }; 
typedef struct {
    float activeLeft;
    float cooldown;
    WarpState state;
} WarpBoost;

typedef struct {
    Vector2 position;
    Vector2 v;
    float radius;
    float alpha;
    Color color;
} CircleWave;

typedef struct {
    Vector2 position;
    float radius;
    float alpha;
    float gravity_lines_shift;
    Color color;
    WarpBoost warp;
} Player;

void InitCircle(CircleWave& cir, int width, int height, float difficulty);
void InitPlayer(Player& plr);
CircleWave* InitPlanets(CircleWave* planets, int screenWidth, int screenHeight, float difficulty);