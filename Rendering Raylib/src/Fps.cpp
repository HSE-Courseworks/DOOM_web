#include "Fps.hpp"

Fps::Fps(const char* textFont) : font()
{
	font = LoadFont(textFont);
}

void Fps::show() const
{
	const char *textFps = TextFormat("%i FPS", GetFPS());
	Vector2 bounds = MeasureTextEx(font, textFps, FONT_SIZE, SPACING);
	Vector2 posFPS = {GetRenderWidth() - SHIFT - bounds.x, SHIFT};
	DrawTextEx(font, textFps, posFPS, FONT_SIZE, SPACING, BLACK);
}
