#ifndef TIMER_HPP
#define TIMER_HPP

#include "raylib.h"
#include <SFML/Network.hpp>

#define TIMER_X (860)
#define TIMER_Y (20)
#define TIMER_SIZE_X (200)
#define TIMER_SIZE_Y (60)
#define THICKNESS (10)
#define FONT_SIZE_TIMER (40)

class Timer
{
public:
    Timer(const int duration);
    void start();
    void stop();
    void update();
    void show() const;
    void reboot();

    int getLeftSeconds() const;
    friend sf::Packet& operator<<(sf::Packet& pack, Timer& timer);
private:
    int duration, leftTime;
    bool isStart = false;
    double prevTime;
    Rectangle backGround;
    Texture2D clockTexture;
    Font font;
};

sf::Packet& operator<<(sf::Packet& pack, Timer& timer);

#endif