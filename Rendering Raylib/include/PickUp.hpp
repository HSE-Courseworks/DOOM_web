#ifndef PICKUP_HPP
#define PICKUP_HPP

#include "raylib.h"

#define TIME_REBIRTH (10)

class PickUp {
public:
    PickUp() = default;
    PickUp(char symbol, const float radius, const Vector2& pos, Texture* texture);
    PickUp(const PickUp& other);
    void show(Vector2 shift) const;

    char getSymbol() const;
    float getRadius() const;
    const Vector2& getPosition() const;
    void setFlagActive(bool flag);
    bool getFlagActive() const;
    void setTimeGet(int time);
    int getTimeGet() const;
    int getHowMuchAdd() const;
    const Texture2D *getTexture() const;

private:
    char symbol;
    float radius;
    bool isActive;
    int timeGet;
    int howMuchAdd = 20;
    Texture2D* texture;
    Vector2 position;
};

#endif