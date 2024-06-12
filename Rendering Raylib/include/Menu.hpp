#ifndef MENU_HPP
#define MENU_HPP

#include "Page.hpp"

#define BUTTON_WIDTH_MENU (800)
#define BUTTON_HEIGHT_MENU (120)

class Menu : Page
{
public:
    Menu();
    Pages update() override;
    void show() const override;

private:
    Rectangle startButton, settingsButton, exitButton;
    Color colorStart, colorSettings, colorExit;
    Texture2D title;
    Font font;
};

#endif