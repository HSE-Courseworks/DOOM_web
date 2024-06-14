#include "PickUp.hpp"

PickUp::PickUp(const char symbol, const Vector2 &pos, Texture *texture)
    : symbol(symbol), isActive(true), timeGet(0), texture(texture), position(pos) {}

PickUp::PickUp(const PickUp &other)
{
    symbol = other.symbol;
    position = other.position;
    isActive = other.isActive;
    texture = other.texture;
}

void PickUp::show(const Vector2& shift) const
{
    DrawCircle(position.x + shift.x, position.y + shift.y, SIZE_PICKUP, BLACK);
}

void PickUp::setSymbol(const char newSymbol) { symbol = newSymbol; }

void PickUp::setPosition(const Vector2 &newPosition) { position = newPosition; }

void PickUp::setFlagActive(bool flag) { isActive = flag; };

void PickUp::setTimeGet(const int time) { timeGet = time; }

char PickUp::getSymbol() const { return symbol; }

const Vector2 &PickUp::getPosition() const { return position;}

bool PickUp::getFlagActive() const { return isActive; }

int PickUp::getTimeGet() const { return timeGet; }

int PickUp::getHowMuchAdd() const { return howMuchAdd; }

const Texture2D *PickUp::getTexture() const { return texture; }