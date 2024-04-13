#ifndef FPS_HPP
#define FPS_HPP

#include "raylib.h"
#include "Tools.hpp"
#include <string>

#define FONT_SIZE (25)
#define SPACING (2)
#define SHIFT_X (15)
#define SHIFT_Y (20)

class Fps
{
public:
	Fps();
	Fps(const std::string& textFont);
	void show() const;
private:
	Font font;
};

#endif 