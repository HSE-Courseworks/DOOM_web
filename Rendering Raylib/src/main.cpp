#include "raylib.h"
#include "ScreenSaver.hpp"
#include <iostream>
#include <cmath>

#define WIDTH_RENDER (1920)
#define HEIGHT_RENDER (1080)

int main()
{
    InitWindow(WIDTH_RENDER, HEIGHT_RENDER, "Raycasting 3D game");
    InitAudioDevice(); SetExitKey(KEY_NULL);
    SetConfigFlags(FLAG_VSYNC_HINT); SetTargetFPS(60);

    // Все, кто предоставит никнейм, будут считаться объектом класса User
    // `active` показывает, активен ли сейчас user или нет
    // ScreenSaver - объект по управлению вывода информации для user
    std::vector<ScreenSaver*> screenSavers;
    screenSavers.push_back(new ScreenSaver(User(1, "Lolbl4"))); 
    screenSavers.push_back(new ScreenSaver(User(2, "Sadness")));
    screenSavers.push_back(new ScreenSaver(User(3, "Nif-naf")));
    int cur = 0;

    World world("resources/maze.txt", "resources/textures.txt");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (screenSavers[cur]->getPage() == Pages::GAME && !screenSavers[cur]->getUser().getFlagInGame()) {
            screenSavers[cur]->getUser().setFlagInGame(true);

            int id = screenSavers[cur]->getUser().getId();
            std::string nickName = screenSavers[cur]->getUser().getNickName();
            world.addPlayer(id, nickName);
            screenSavers[cur]->setWorld(&world);
            world.timer.start();
        }
        else if (screenSavers[cur]->getPage() == Pages::MENU && screenSavers[cur]->getUser().getFlagInGame()) {
            screenSavers[cur]->getUser().setFlagInGame(false);
            int id = screenSavers[cur]->getUser().getId();
            world.removePlayer(id);
            screenSavers[cur]->setWorld(nullptr);
            if (!world.getPlayersNumber()) {
                world.reboot();
                world.timer.stop();
            }
            // Игра закончена или все отключились, 
            // имеет смысл привести world в исходное состояние
        }

        bool exit = screenSavers[cur]->update();
        screenSavers[cur]->show();

        if (!world.getTimeEnd() && !world.timer.getLeftSeconds()) world.setTimeEnd(GetTime());
        if (exit) break;

        // Переключить user (нужно только для отладки, при мультиплеере можно убрать)
        if (IsKeyReleased(KEY_V)) {
            cur = (cur + 1) % screenSavers.size(); 
            if (screenSavers[cur]->getPage() == Pages::GAME || 
            screenSavers[cur]->getPage() == Pages::RESURRECTION) HideCursor();
            else ShowCursor();
        }

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
