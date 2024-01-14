#include "Fps.hpp"

Fps::Fps(std::string _font) : curFps(0), font()
{
	font.loadFromFile(_font);
}

void Fps::update(unsigned int newFps)
{
	curFps = newFps;
}

void Fps::show(RenderWindow& window)
{
	Text text("FPS: " + std::to_string(curFps), font, SIZE_FPS_FRAME / 2);
	text.setFillColor(Color::Black);
	text.setPosition(Vector2f(window.getSize().x - text.getGlobalBounds().width - SHIFT, SHIFT));
	window.draw(text);
}
