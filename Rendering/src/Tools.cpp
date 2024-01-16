#include "Tools.hpp"

using namespace sf;

float DegToRad(float angle)
{
    return angle * M_PI / 180;
}

float RadToDeg(float angle)
{
    return angle * 180 / M_PI;
}

float getDist2Points(const Vector2f& v1, const Vector2f& v2)
{
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

Color changeLightness(Color color, int delta)
{
    int r = std::max(color.r + 2 * delta / 3, 0);
    int g = std::max(color.g + 2 * delta / 3, 0);
    int b = std::max(color.b + 2 * delta / 3, 0);
    return Color(r, g, b);
}
