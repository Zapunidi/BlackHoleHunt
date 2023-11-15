#include "raylib.h"

typedef struct {
    float ActiveLeft;
    float Cooldown;
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