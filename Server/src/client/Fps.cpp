#include "Fps.hpp"

Fps::Fps()
{
	//font = LoadFontEx("resources/Calibri.ttf", FONT_SIZE_FPS, nullptr, 0);
}

void Fps::show() const
{
	const char *textFps = TextFormat("%d FPS", GetFPS());

	Vector2 bounds = MeasureTextEx(font, textFps, FONT_SIZE_FPS, SPACING_FPS);
	Rectangle frame = {GetRenderWidth() - 2 * SHIFT_FPS - bounds.x, SHIFT_FPS,
					   SHIFT_FPS + bounds.x, SHIFT_FPS + bounds.y};

	Vector2 posFPS = {frame.x + (frame.width - bounds.x) / 2.0f,
					  frame.y + (frame.height - bounds.y) / 2.0f};

	DrawRectangleRec(frame, softGray);
	DrawTextEx(font, textFps, posFPS, FONT_SIZE_FPS, SPACING_FPS, tintText);
}
