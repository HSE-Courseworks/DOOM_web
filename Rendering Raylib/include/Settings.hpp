#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Page.hpp"

#include <vector>
#include <string>
#include <algorithm>

#define VOLUME_BAR_WIDTH (250)
#define VOLUME_BAR_HEIGHT (40)
#define VOLUME_SLIDER_WIDTH (25)
#define BACK_BUTTON_X (100)
#define BACK_BUTTON_Y (130)
#define BACK_BUTTON_RADIUS (50)
#define FRAME_WIDTH (800)
#define FRAME_HEIGTH (700)
#define SHIFT_DESC (400)

class Settings : Page
{
public:
    Settings();
    void show() const override;
    Pages update() override;

    void setPrevPage(Pages page);
    float getVolume();

private:
    float volume; Pages prevPage;
    Rectangle volumeSliderBar, volumeSlider, frame;
    Color colorBack;
    Texture2D back, title;
    Font font;
};

#endif