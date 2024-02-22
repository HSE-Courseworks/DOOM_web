#ifndef FPS_HPP
#define FPS_HPP

#include "raylib.h"
#include <string>

#define FONT_SIZE (25)
#define SPACING (2)
#define SHIFT (10)

class Fps
{
public:
	Fps(const char* textFont);
	void show() const;
private:
	Font font;
};

#endif 