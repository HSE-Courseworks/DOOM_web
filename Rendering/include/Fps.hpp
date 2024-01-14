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
	Fps(std::string _font);
	void update(unsigned int newFps);
	void show(RenderWindow& window);
private:
	Font font;
	unsigned int curFps;
};

#endif 