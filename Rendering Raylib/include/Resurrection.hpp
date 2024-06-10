#ifndef RESURRECTION_HPP
#define RESURRECTION_HPP

#include "Page.hpp"
#include <string>

#define TIME_RESURRECTION (15 + 1)

class Resurrection : Page {
public:
    Resurrection();
    void show() const override;
    Pages update() override;
    void setTimeDied(const double time);

private:
    float whenDied = 0;
    unsigned char alpha, step;
    Font font;
    Texture2D backGround, title;
};

#endif