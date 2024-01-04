#include "main.h"

void InitCredits(int screenWidth, int screenHeight);
enum ProgramState ProcessCredits(enum ProgramState inState, float dt);
void DrawCredits(const int screenWidth, const int screenHeight);
void UnloadCredits(void);