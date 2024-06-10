#include "Fps.hpp"

Fps::Fps()
{
	font = LoadFontEx("resources/Calibri.ttf", 30, nullptr, 0);
}

void Fps::show() const
{
	const char *textFps = TextFormat("%i FPS", GetFPS());
	Vector2 bounds = MeasureTextEx(font, textFps, FONT_SIZE_FPS, SPACING);
	Vector2 posFPS = {GetRenderWidth() - 3.0f / 2.0f * SHIFT_X - bounds.x, 3.0f / 2.0f * SHIFT_Y};
	DrawRectangle(posFPS.x - 0.5 * SHIFT_X, posFPS.y - 0.5 * SHIFT_Y, 
				  bounds.x + 3.0f / 2.0f * SHIFT_X, bounds.y + 0.5 * SHIFT_Y, softGray);
	DrawTextEx(font, textFps, posFPS, FONT_SIZE_FPS, SPACING, tintText);
}
