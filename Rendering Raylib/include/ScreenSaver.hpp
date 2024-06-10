#ifndef SCREEN_SAVER_HPP
#define SCREEN_SAVER_HPP

#include "Menu.hpp"
#include "Settings.hpp"
#include "Resurrection.hpp"
#include "Pause.hpp"
#include "Summary.hpp"
#include "User.hpp"
#include "World.hpp"

#define WIDTH_RENDER (1920)
#define HEIGHT_RENDER (1080)

class ScreenSaver
{
public:
    ScreenSaver(const User &user);
    ~ScreenSaver();
    bool update();
    void show() const;

    void setPage(const Pages& page);
    void setWorld(World *newWorld);
    const User &getUser() const;
    User &getUser();
    Pages getPage() const;

private:
    Pages prevPage = Pages::MENU, curPage = Pages::MENU;
    User user;
    Menu menu; Settings settings; World *world;
    Resurrection resurrection; Pause pause; Summary summary;
    Music music;
    Texture2D mainBackGround;
};

#endif