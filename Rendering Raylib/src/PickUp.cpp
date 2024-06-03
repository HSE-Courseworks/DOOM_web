#include "PickUp.hpp"

PickUp::PickUp(char symbol, const float radius, const Vector2& pos, Texture* texture)
    : symbol(symbol), radius(radius), isActive(true), timeGet(0), texture(texture), position(pos) {}

PickUp::PickUp(const PickUp& other) {
    symbol = other.symbol;
    radius = other.radius;
    position = other.position;
    isActive = other.isActive;
    timeGet = other.timeGet;
    texture = other.texture;
}

void PickUp::show(Vector2 shift) const {
    DrawCircle(position.x + shift.x, position.y + shift.y, radius, BLACK);
}

char PickUp::getSymbol() const {
    return symbol;
}

float PickUp::getRadius() const {
    return radius;
}

const Vector2& PickUp::getPosition() const {
    return position;
}

void PickUp::setFlagActive(bool flag) {
    isActive = flag;
}

bool PickUp::getFlagActive() const {
    return isActive;
}

void PickUp::setTimeGet(int time) {
    timeGet = time;
}

int PickUp::getTimeGet() const {
    return timeGet;
}

int PickUp::getHowMuchAdd() const {
    return howMuchAdd;
}

const Texture2D* PickUp::getTexture() const {
    return texture;
}