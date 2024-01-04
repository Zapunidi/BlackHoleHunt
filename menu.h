#include "main.h"

void InitMenu(int screenWidth, int screenHeight);
enum ProgramState ProcessMenu(enum ProgramState inState, float dt);
void DrawMenu(const int screenWidth, const int screenHeight);
void UnloadMenu(void);