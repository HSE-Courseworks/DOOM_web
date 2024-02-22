#ifndef TOOLS_HPP
#define TOOLS_HPP

#define _USE_MATH_DEFINES
#include "raylib.h"
#include <cmath>
#include <math.h>
#include <string>

// const Color softGray = { 50, 54, 52 };
// const Color softGreen = { 40, 237, 142 };
const Color lightGray = { 175, 175, 175, 255 };
const Color softRed = { 245, 71, 71, 255 };
const Color colorSky = { 128, 0, 0, 255 };
const Color semiTransparentWhite = { 176, 176, 176, 10 };
const Color swamp = { 52, 59, 41, 255 };
const Color darkSwamp = { 23, 31, 9, 255 };

float getDist2Points(const Vector2& v1, const Vector2& v2);
float DegToRad(const float angle);
float RadToDeg(const float angle);
Color changeLightness(const Color& color, const int delta);

#endif 