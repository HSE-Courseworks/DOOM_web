#include "Fps.hpp"

Fps::Fps()
{
	font = LoadFontEx("resources/Calibri.ttf", 30, nullptr, 0);
}

void Fps::show() const
{
	const char *textFps = TextFormat("%i FPS", GetFPS());
	Vector2 bounds = MeasureTextEx(font, textFps, FONT_SIZE, SPACING);
	Vector2 posFPS = {GetRenderWidth() - SHIFT_X - bounds.x, SHIFT_Y};
	DrawRectangle(posFPS.x - 5, posFPS.y - 5, bounds.x + 10, bounds.y + 5, softGray);
	DrawTextEx(font, textFps, posFPS, FONT_SIZE, SPACING, tintText);
}
