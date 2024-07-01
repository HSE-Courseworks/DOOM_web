/*#include "raylib.h"
#include "ScreenSaver.hpp"

#include "World.hpp"
#include "Fps.hpp"
//#include "Client.hpp"
#include <iostream>

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
    screenSavers.push_back(new ScreenSaver(User(1, "Lolbl4")));
    //screenSavers.push_back(new ScreenSaver(User(2, "Sadness")));
    //screenSavers.push_back(new ScreenSaver(User(3, "Xlopchik13")));
    int cur = 0;

    sf::TcpSocket TCPsock;
    sf::UdpSocket UDPsock;
    if (TCPsock.connect(ServIP, TCPPort) != sf::TcpSocket::Done)
        TraceLog(LOG_ERROR, "Connection error. The client has not connected to a serv.\n\n\n");
    UDPsock.bind(ClientUDPPort);
    sf::Packet pack;
    TCPsock.receive(pack);
    pack >> cur;
    pack.clear();
    cur = 0;
    TCPsock.setBlocking(false);
    UDPsock.setBlocking(false);
    while (!WindowShouldClose())
    {
        CheckNewData(TCPsock, UDPsock, world, screenSavers);
        BeginDrawing();
        ClearBackground(BLACK);

        if (screenSavers[cur]->getPage() == Pages::GAME && !screenSavers[cur]->getUser().getFlagInGame()) {
            int id = screenSavers[cur]->getUser().getId();
            std::string nickName = screenSavers[cur]->getUser().getNickName();

            screenSavers[cur]->getUser().setFlagInGame(true);
            world.addPlayer(id, nickName);
            screenSavers[cur]->setWorld(&world);
            world.timer.start();
        }
        else if (screenSavers[cur]->getPage() == Pages::MENU && screenSavers[cur]->getUser().getFlagInGame()) {
            int id = screenSavers[cur]->getUser().getId();
            screenSavers[cur]->getUser().setFlagInGame(false);
            world.removePlayer(id);
            screenSavers[cur]->setWorld(nullptr);
            if (!world.getPlayersNumber()) { world.reboot(); world.timer.stop(); }

            // Игра закончена или все игроки отключились =>
            // имеет смысл привести объект класса World в исходное состояние
        }

        bool exit = screenSavers[cur]->update();
        screenSavers[cur]->show();

        if (!world.getTimeEnd() && !world.timer.getLeftSeconds()) world.setTimeEnd(GetTime());
        if (exit) break;

        // Переключить user (нужно только для отладки, при мультиплеере нужно убрать)
        if (IsKeyReleased(KEY_V)) {
            cur = (cur + 1) % screenSavers.size();
            Pages curPage = screenSavers[cur]->getPage();
            if (curPage == Pages::GAME || curPage == Pages::RESURRECTION) HideCursor();
            else ShowCursor();
        }

        EndDrawing();
        SendData(UDPsock, world, cur);
    }
    pack << PLAYER_DISCONNECTED << cur;
    TCPsock.send(pack);

    world.removePlayer(1); //game.removePlayer(2); 
    // game.removePlayer(3); // game.removePlayer(4);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
*/