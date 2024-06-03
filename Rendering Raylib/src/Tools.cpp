#include "Tools.hpp"

float DegToRad(const float angle)
{
    return angle * M_PI / 180;
}

float RadToDeg(const float angle)
{
    return angle * 180 / M_PI;
}

float getDist2Points(const Vector2& v1, const Vector2& v2)
{
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

Color changeLightness(const Color& color, const int delta)
{
    unsigned char r = std::max(color.r + (int)(delta * (5.0f / 4.0f)), 0);
    unsigned char g = std::max(color.g + (int)(delta * (5.0f / 4.0f)), 0);
    unsigned char b = std::max(color.b + (int)(delta * (5.0f / 4.0f)), 0);
    return {r, g, b, 255};
}

double constrainAngle360(const double angle) 
{
    return std::fmod(std::fmod(angle, 360.0) + 360.0, 360.0);
}