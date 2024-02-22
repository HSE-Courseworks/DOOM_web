#include "raylib.h"
#include "World.hpp"
#include "Fps.hpp"
#include <iostream>
#include <cmath>

// Weapon
// Bots
// Gameplay

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1920, 1080, "New DOOM");
    HideCursor(); SetTargetFPS(60);
  
    SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
    Fps curFps("resources/alagard.png");
    World game("resources/maze.txt");

    while (!WindowShouldClose())
    {
        if (IsKeyReleased(KEY_M)) 
            game.setFlagMiniMap(!game.getFlagMiniMap());

        BeginDrawing();
        ClearBackground(colorSky);
        game.updateWorld(GetFrameTime());
        game.showWorld();
        curFps.show();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}