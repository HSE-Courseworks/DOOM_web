#include "Fps.hpp"

Fps::Fps(const std::string& textFont) : curFps(0), font()
{
	font.loadFromFile(textFont);
}

void Fps::update(const unsigned short newFps)
{
	curFps = newFps;
}

void Fps::show(RenderWindow& window) const
{
	Text text("FPS: " + std::to_string(curFps), font, SIZE_FPS_FRAME / 2.0f);
	text.setFillColor(Color::Black);
	text.setPosition(Vector2f(window.getSize().x - text.getGlobalBounds().width - SHIFT, SHIFT));
	window.draw(text);
}
