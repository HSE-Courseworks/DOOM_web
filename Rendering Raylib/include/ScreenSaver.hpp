#ifndef SCREEN_SAVER_HPP
#define SCREEN_SAVER_HPP

#include "Menu.hpp"
#include "Settings.hpp"
#include "Resurrection.hpp"
#include "Pause.hpp"
#include "Summary.hpp"
#include "User.hpp"
#include "World.hpp"

class ScreenSaver {
public:
    ScreenSaver(const User& user);
    bool update();
    void show() const;
    ~ScreenSaver();
    Pages getPage() const;
    void setPage(Pages page);
    void setWorld(World *newWorld);
    const User& getUser() const;
    User& getUser();

private:
    Pages prevPage = Pages::MENU, curPage = Pages::MENU; User user;

    Menu menu; Settings settings; World* world;
    Resurrection resurrection; Pause pause; Summary summary;
    Music music; Texture2D mainBackGround;
};

#endif