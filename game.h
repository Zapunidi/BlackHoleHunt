#include "main.h"

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
void SetSound(Sound soundEat);

void InitGame(int screenWidth, int screenHeight);
ProgramState ProcessGame(ProgramState inState, float dt);
void DrawGame(const int screenWidth, const int screenHeight);
void UnloadGame(void);