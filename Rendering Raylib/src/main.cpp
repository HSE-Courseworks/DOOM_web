#include "raylib.h"
#include "World.hpp"
#include "Fps.hpp"
#include <iostream>
#include <cmath>

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1920, 1080, "Raycasting 3D game");
    HideCursor(); SetTargetFPS(60);
    InitAudioDevice();

    SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
    Fps curFps; World game("resources/maze.txt", "resources/textures.txt");

    Vector2 pos_1;
    pos_1.x = game.gameMap.getFrame().x + game.gameMap.getWallSize().x * 3 / 2.0f;
    pos_1.y = game.gameMap.getFrame().y + game.gameMap.getWallSize().y * 3 / 2.0f;
    Vector2 pos_2;
    pos_2.x = game.gameMap.getFrame().x + game.gameMap.getWallSize().x * (game.gameMap.getMazeSize().x - 3 / 2.0f);
    pos_2.y = game.gameMap.getFrame().y + game.gameMap.getWallSize().y * (game.gameMap.getMazeSize().y - 3 / 2.0f);
    // Vector2 pos_3;
    // pos_3.x = game.gameMap.getFrame().x + game.gameMap.getWallSize().x * (game.gameMap.getMazeSize().x - 3 / 2.0f);
    // pos_3.y = game.gameMap.getFrame().y + game.gameMap.getWallSize().y * 3 / 2.0f;
    // Vector2 pos_4;
    // pos_4.x = game.gameMap.getFrame().x + game.gameMap.getWallSize().x * 3 / 2.0f;
    // pos_4.y = game.gameMap.getFrame().y + game.gameMap.getWallSize().y * (game.gameMap.getMazeSize().y - 3 / 2.0f);

    game.addPlayer(Player(pos_1, 0, softRed, "resources/player.png", "Lolbl4"));
    game.addPlayer(Player(pos_2, 180, softBlue, "resources/player.png", "Sandess"));
    // game.addPlayer(Player(pos_3, 180, softPink, "resources/player.png", "Nif-Naf"));
    // game.addPlayer(Player(pos_4, 0, softYellow, "resources/player.png", "Sigmoid"));

    Music music = LoadMusicStream("resources/backGroundMusic.mp3");
    music.looping = true;
    SetMusicVolume(music, 1);
    PlayMusicStream(music);
    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(BLACK);
        game.updateWorld(GetFrameTime());
        if (!game.timer.getLeftSeconds()) break;
        game.showWorld();
        curFps.show();
        EndDrawing();
    }
    game.removePlayer(1); game.removePlayer(2); 
    // game.removePlayer(3); // game.removePlayer(4);

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
