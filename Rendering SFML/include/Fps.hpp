#ifndef FPS_HPP
#define FPS_HPP

#include <SFML/Graphics.hpp>
#include "Tools.hpp"

using namespace sf;

#define SIZE_FPS_FRAME (50)
#define SHIFT (10)

class Fps
{
public:
	Fps(const std::string& textFont);
	void update(const unsigned short newFps);
	void show(RenderWindow& window) const;
private:
	Font font;
	unsigned short curFps;
};

#endif 