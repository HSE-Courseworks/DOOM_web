#ifndef PICKUP_HPP
#define PICKUP_HPP

#include "raylib.h"

#define TIME_REBIRTH (10)
#define SIZE_PICKUP (2.0f)

class PickUp
{
public:
    PickUp() = default;
    PickUp(const char symbol, const Vector2 &pos, Texture *texture);
    PickUp(const PickUp &other);
    void show(const Vector2 &shift) const;

    void setSymbol(const char newSymbol);
    void setPosition(const Vector2 &newPosition);
    void setFlagActive(bool flag);
    void setTimeGet(int time);

    char getSymbol() const;
    const Vector2 &getPosition() const;
    bool getFlagActive() const;
    int getTimeGet() const;
    int getHowMuchAdd() const;
    const Texture2D *getTexture() const;

private:
    char symbol;
    bool isActive;
    int timeGet, howMuchAdd = 20;
    Texture2D *texture;
    Vector2 position;
};

#endif