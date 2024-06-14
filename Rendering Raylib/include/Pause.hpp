#ifndef PAUSE_HPP
#define PAUSE_HPP

#include "Page.hpp"
#include "Tools.hpp"

#define BACK_WIDTH (620)
#define BACK_HEIGHT (480)

class Pause : Page
{
public:
    Pause();
    void show() const override;
    Pages update() override;
    void setPrevPage(Pages page);

private:
    Rectangle backGround, resumeButton, settingsButton, exitButton;
    Color colorResume, colorSettings, colorExit;
    Font font;
    Pages prevPage;
};

#endif