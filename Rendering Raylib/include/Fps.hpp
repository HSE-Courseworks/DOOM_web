#ifndef FPS_HPP
#define FPS_HPP

#include "raylib.h"
#include "Tools.hpp"
#include <string>

#define FONT_SIZE (25)
#define SPACING (2)
#define SHIFT_X (10)
#define SHIFT_Y (10)

class Fps
{
public:
	Fps();
	void show() const;
private:
	Font font;
};

#endif 