#include "Timer.hpp" 

Timer::Timer(const int time) : time(time) {
    font = LoadFontEx("resources/Calibri.ttf", 40, nullptr, 0);
    backGround = {TIMER_X, TIMER_Y, TIMER_SIZE_X, TIMER_SIZE_Y};
    prevTime = GetTime();
    clockTexture = LoadTexture("resources/watch.png");
}

void Timer::update() {
    double curTime = GetTime();
    if (curTime - prevTime >= 1.0) {
        time--;
        prevTime = curTime;
    }
}

void Timer::show() const {
    const char *textInfo = TextFormat("%02i:%02i", time / 60, time % 60);
    Vector2 bounds = MeasureTextEx(font, textInfo, 40, 0);
    DrawRectangleRec(backGround, GRAY);
    DrawRectangleLinesEx(backGround, THICKNESS, BLACK);
    DrawTextEx(font, textInfo, {TIMER_X + (TIMER_SIZE_X - bounds.x) / 2.0f, 
                TIMER_Y + THICKNESS / 2 + (TIMER_SIZE_Y - bounds.y) / 2.0f}, 40, 0, BLACK);
    DrawTexture(clockTexture, TIMER_X + THICKNESS, TIMER_Y + THICKNESS, WHITE);
    DrawTexture(clockTexture, TIMER_X + TIMER_SIZE_X - clockTexture.width - THICKNESS, TIMER_Y + THICKNESS, WHITE);
}

int Timer::getLeftSeconds() const {
    return time;
}