#include "raylib.h"
#include "World.hpp"
#include "Fps.hpp"
#include "Client.hpp"
#include <iostream>
#include <cmath>
#include <thread>

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1920, 1080, "Raycasting 3D game");
    HideCursor(); SetTargetFPS(60);
    InitAudioDevice();

    SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
    Fps curFps; World game("resources/maze.txt", "resources/textures.txt");

    const Vector2 sizePlayer = {game.gameMap.getWallSize().x / 4.0f, game.gameMap.getWallSize().x / 4.0f};
    Vector2 pos_1;
    pos_1.x = game.gameMap.getFrame().x + game.gameMap.getWallSize().x * 3 / 2.0f;
    pos_1.y = game.gameMap.getFrame().y + game.gameMap.getWallSize().y * 3 / 2.0f;
    //Vector2 pos_2;
    //pos_2.x = game.gameMap.getFrame().x + game.gameMap.getWallSize().x * (game.gameMap.getMazeSize().x - 3 / 2.0f);
    //pos_2.y = game.gameMap.getFrame().y + game.gameMap.getWallSize().y * (game.gameMap.getMazeSize().y - 3 / 2.0f);
    // Vector2 pos_3;
    // pos_3.x = game.gameMap.getFrame().x + game.gameMap.getWallSize().x * (game.gameMap.getMazeSize().x - 3 / 2.0f);
    // pos_3.y = game.gameMap.getFrame().y + game.gameMap.getWallSize().y * 3 / 2.0f;
    // Vector2 pos_4;
    // pos_4.x = game.gameMap.getFrame().x + game.gameMap.getWallSize().x * 3 / 2.0f;
    // pos_4.y = game.gameMap.getFrame().y + game.gameMap.getWallSize().y * (game.gameMap.getMazeSize().y - 3 / 2.0f);

    game.addPlayer(Player(sizePlayer, pos_1, 0, softRed, "resources/player_1.png"));
    //game.addPlayer(Player(sizePlayer, pos_2, 180, softBlue, "resources/player_2.png"));
    // game.addPlayer(Player(sizePlayer, pos_3, 180, softPink, "resources/player_3.png"));
    // game.addPlayer(Player(sizePlayer, pos_4, 0, softYellow, "resources/player_4.png"));

    Music music = LoadMusicStream("resources/backGroundMusic.mp3");
    music.looping = true;
    SetMusicVolume(music, 1);
    PlayMusicStream(music);
    sf::TcpSocket TCPsock;
    sf::UdpSocket UDPsock;
    if (TCPsock.connect(ServIP, TCPPort) != sf::TcpSocket::Done)
        TraceLog(LOG_ERROR, "Connection error. The client has not connected to a serv.");
    UDPsock.bind(ClientUDPPort);
    TCPsock.setBlocking(false);
    //UDPsock.setBlocking(false);
    while (!WindowShouldClose())
    {
        CheckNewData(TCPsock, UDPsock, game);
        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(BLACK);
        game.updateWorld(GetFrameTime());
        game.showWorld();
        curFps.show();
        EndDrawing();
        SendData(UDPsock);
    }
    sf::Packet pack;
    pack << PLAYER_DISCONNECTED << game.GetCurPlayerID();
    TCPsock.send(pack);

    game.removePlayer(1); //game.removePlayer(2); 
    // game.removePlayer(3); // game.removePlayer(4);
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
