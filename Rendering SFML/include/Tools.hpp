#ifndef TOOLS_HPP
#define TOOLS_HPP

#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <cmath>
#include <math.h>
#include <string>

using namespace sf;

// const Color softGray = { 50, 54, 52 };
// const Color softGreen = { 40, 237, 142 };
const Color lightGray = { 175, 175, 175 };
const Color softRed = { 245, 71, 71 };
const Color colorSky = { 85, 0, 0 };
const Color semiTransparentWhite = { 176, 176, 176, 80 };
const Color swamp = { 52, 59, 41 };
const Color darkSwamp = { 23, 31, 9 };
const std::string helvetica = "resources/Helvetica.ttf";

float getDist2Points(const Vector2f& v1, const Vector2f& v2);
float DegToRad(const float angle);
float RadToDeg(const float angle);
Color changeLightness(const Color& color, const int delta);

#endif 