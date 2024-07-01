#include "ScreenSaver.hpp"

ScreenSaver::ScreenSaver(const User &user) : 
    user(user), menu(), settings(), world(nullptr), resurrection(), pause(), summary()
{
    SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
    //mainBackGround = LoadTexture("resources/backGround.png");
    //music = LoadMusicStream("resources/backGroundMusic.mp3");
    music.looping = true;
    PlayMusicStream(music);
}

bool ScreenSaver::update()
{
    UpdateMusicStream(music);
    bool exit = false; prevPage = curPage;

    switch (curPage)
    {
    case MENU: curPage = menu.update(); break;
    case SETTINGS: curPage = settings.update(); break;
    case RESURRECTION: curPage = resurrection.update(); break;
    case PAUSE: curPage = pause.update(); break;
    case SUMMARY: curPage = summary.update(); break;
    case GAME: if (world) {curPage = world->update(user.getId(), GetFrameTime()); } break;
    case EXIT: exit = true; break;
    }

    if ((prevPage == Pages::MENU || prevPage == Pages::PAUSE) && curPage == Pages::SETTINGS)
        settings.setPrevPage(prevPage);

    else if ((prevPage == Pages::GAME || prevPage == Pages::RESURRECTION) && curPage == Pages::PAUSE)
        pause.setPrevPage(prevPage);

    else if (prevPage == Pages::GAME && curPage == Pages::RESURRECTION)
        resurrection.setTimeDied(world->players[user.getId()].second->getTimeDied());

    else if (prevPage == Pages::RESURRECTION && curPage == Pages::GAME)
        world->resurrectPlayer(user.getId());

    else if (prevPage == Pages::GAME && curPage == Pages::SUMMARY) {
        summary.setTimeEnd(world->getTimeEnd());
        summary.setFinalScoreTable(world->scoreTable);
    }

    SetMasterVolume(settings.getVolume());

    return exit;
}

void ScreenSaver::show() const
{
    if (curPage == Pages::MENU || curPage == Pages::SETTINGS) {
        DrawTexture(mainBackGround, 0, 0, WHITE);
    }

    switch (curPage)
    {
    case MENU: menu.show(); break;
    case SETTINGS: settings.show(); break;
    case RESURRECTION: resurrection.show(); break;
    case PAUSE: pause.show(); break;
    case SUMMARY: summary.show(); break;
    case GAME: if (world) { world->show(user.getId()); } break;
    case EXIT: break;
    }
}

void ScreenSaver::setPage(const Pages& page) { curPage = page; }

void ScreenSaver::setWorld(World *newWorld) { world = newWorld; }

Pages ScreenSaver::getPage() const { return curPage; }

const User &ScreenSaver::getUser() const { return user; }

User &ScreenSaver::getUser() { return user; }

ScreenSaver::~ScreenSaver() { UnloadMusicStream(music); }