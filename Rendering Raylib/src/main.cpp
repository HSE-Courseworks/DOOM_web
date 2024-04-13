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
    HideCursor(); 
    SetTargetFPS(60);
  
    SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
    Fps curFps("resources/romulus.png");
    //Fps curFps;
    World game("resources/maze.txt", "resources/textures.txt");

    const Vector2 sizePlayer = {game.gameMap.getWallSize().x / 4, game.gameMap.getWallSize().x / 4};
    Vector2 pos_1;
    pos_1.x = game.gameMap.getFrame().x + game.gameMap.getWallSize().x * 3 / 2.0f;
    pos_1.y = game.gameMap.getFrame().y + game.gameMap.getWallSize().y * 3 / 2.0f;
    Vector2 pos_2;
    pos_2.x = game.gameMap.getFrame().x + SIZE_PIXEL_MAP - game.gameMap.getWallSize().x * 3 / 2.0f;
    pos_2.y = game.gameMap.getFrame().y + SIZE_PIXEL_MAP - game.gameMap.getWallSize().y * 3 / 2.0f;
    Vector2 pos_3;
    pos_3.x = game.gameMap.getFrame().x + SIZE_PIXEL_MAP - game.gameMap.getWallSize().x * 3 / 2.0f;
    pos_3.y = game.gameMap.getFrame().y + game.gameMap.getWallSize().y * 3 / 2.0f;
    // Vector2 pos_4;
    // pos_4.x = game.gameMap.getFrame().x + game.gameMap.getWallSize().x * 3 / 2.0f;
    // pos_4.y = game.gameMap.getFrame().y + SIZE_PIXEL_MAP - game.gameMap.getWallSize().y * 3 / 2.0f;

    game.addPlayer(Player(sizePlayer, pos_1, 0, softRed, "resources/player_1.png", "resources/crosshair.png"));
    game.addPlayer(Player(sizePlayer, pos_2, 180, softBlue, "resources/player_2.png", "resources/crosshair.png"));
    game.addPlayer(Player(sizePlayer, pos_3, 180, softPink, "resources/player_3.png", "resources/crosshair.png"));
    // game.addPlayer(Player(sizePlayer, pos_4, 0, softYellow, "resources/player_4.png"));
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(colorSky);
        game.updateWorld(GetFrameTime());
        game.showWorld();
        curFps.show();
        EndDrawing();
    }
    game.removePlayer(0); game.removePlayer(1); 
    game.removePlayer(2); //game.removePlayer(3);
    CloseWindow();
    return 0;
}
