#ifndef FPS_HPP
#define FPS_HPP

#include "Tools.hpp"
#include "raylib.h"

#include <string>

#define FONT_SIZE_FPS (25)
#define SPACING_FPS (5)
#define SHIFT_FPS (10)
class Fps
{
public:
	Fps();
	void show() const;

private:
	Font font;
};

#endif