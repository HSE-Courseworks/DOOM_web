#include "Fps.hpp"

Fps::Fps() : font() {}
Fps::Fps(const std::string& textFont) : font()
{
	font = LoadFont(textFont.data());
}

void Fps::show() const
{
	const char *textFps = TextFormat("%i FPS", GetFPS());
	Vector2 bounds = MeasureTextEx(font, textFps, FONT_SIZE, SPACING);
	Vector2 posFPS = {GetRenderWidth() - SHIFT_X - bounds.x, SHIFT_Y};
	DrawTextEx(font, textFps, posFPS, FONT_SIZE, SPACING, tintText);
}
