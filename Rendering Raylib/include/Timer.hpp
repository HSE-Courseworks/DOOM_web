#ifndef TIMER_HPP
#define TIMER_HPP

#include "raylib.h"

#define TIMER_X (860)
#define TIMER_Y (20)
#define TIMER_SIZE_X (200)
#define TIMER_SIZE_Y (60)
#define THICKNESS (10)

class Timer {
public:
    Timer(const int duration);
    void start();
    void stop();
    void update();
    void show() const;
    void reboot();
    int getLeftSeconds() const;

private:
    int duration, leftTime;
    bool isStart = false;
    double prevTime;
    Rectangle backGround;
    Texture2D clockTexture;
    Font font;
};

#endif