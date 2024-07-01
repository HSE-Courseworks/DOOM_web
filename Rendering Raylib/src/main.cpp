#include "raylib.h"
#include "ScreenSaver.hpp"
#include "Client.hpp"
#include "World.hpp"
#include "Fps.hpp"
#include <iostream>
#include <chrono>

int main() 
{
    InitWindow(WIDTH_RENDER, HEIGHT_RENDER, "3D Strike: C++ Offensive");
    InitAudioDevice();
    SetExitKey(KEY_NULL);
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    // Все, кто предоставит никнейм, будут считаться объектом класса User
    // `active` показывает, активен ли сейчас user или нет (запущен ли у него .exe)
    // ScreenSaver - класс по управлению вывода нужной информации (Pages) для user

    World world("resources/maze.txt", "resources/textures.txt");
    std::vector<ScreenSaver *> screenSavers;
    screenSavers.push_back(new ScreenSaver(User(0, "Lolbl4")));
    //screenSavers.push_back(new ScreenSaver(User(2, "Sadness")));
    //screenSavers.push_back(new ScreenSaver(User(3, "Xlopchik13")));
    int cur = 0;
    bool isConnected = false;
    sf::UdpSocket WorldUDPsock, PlayerUDPsock;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (screenSavers[cur]->getPage() == Pages::GAME && !screenSavers[cur]->getUser().getFlagInGame()) {
            isConnected = ConnectToServ(screenSavers[cur]->getUser().getNickName(), &screenSavers[0]->getUser());
            if (isConnected) {
                WorldUDPsock.bind(ClientWorldUDPPort);
                PlayerUDPsock.bind(ClientPlayerUDPPort);
                ReceiveWorld(WorldUDPsock, &world);
                WorldUDPsock.setBlocking(false);
            }
            int id = screenSavers[cur]->getUser().getId();
            std::string nickName = screenSavers[cur]->getUser().getNickName();

            screenSavers[cur]->getUser().setFlagInGame(true);
            screenSavers[cur]->setWorld(&world);
            if (!isConnected) {
                world.addPlayer(id, nickName);
                world.timer.start();
            }
        }
        else if (screenSavers[cur]->getPage() == Pages::MENU && screenSavers[cur]->getUser().getFlagInGame()) {
            int id = screenSavers[cur]->getUser().getId();
            screenSavers[cur]->getUser().setFlagInGame(false);
            world.removePlayer(id);
            screenSavers[cur]->setWorld(nullptr);
            if (!world.getPlayersNumber()) { world.reboot(); world.timer.stop(); }
            isConnected = false;
            world.sendMessage(PLAYER_DISCONNECTED);

            // Игра закончена или все игроки отключились =>
            // имеет смысл привести объект класса World в исходное состояние
        }

        bool exit = screenSavers[cur]->update();

        screenSavers[cur]->show();

        if (!world.getTimeEnd() && !world.timer.getLeftSeconds()) 
            world.setTimeEnd(GetTime());
        if (exit) 
            break;
        if (isConnected) 
        {
            ReceiveWorld(WorldUDPsock, &world);
            SendPlayer(PlayerUDPsock, world.GetPlayer(screenSavers[cur]->getUser().getId()));
        }

        // Переключить user (нужно только для отладки, при мультиплеере нужно убрать)
        //if (IsKeyReleased(KEY_V)) {
        //    cur = (cur + 1) % screenSavers.size();
        //    Pages curPage = screenSavers[cur]->getPage();
        //    if (curPage == Pages::GAME || curPage == Pages::RESURRECTION) HideCursor();
        //    else ShowCursor();
        //}

        EndDrawing();
    }

    //world.removePlayer(1); world.removePlayer(2); 
    //world.removePlayer(3);  world.removePlayer(4);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
