#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include "raylib.h"
#include "Tools.hpp"

#define SIZE_PIXEL_MAP (240)
#define THICKNESS_MAP (10)

using tableBool = std::vector<std::vector<bool>>;

class Map
{
public:
	tableBool scheme;
	std::vector<Rectangle> objects;

	Map(const char* filename);
	void findObjects();
	void showObjectsInWindow() const;
	void showFrame() const;
	const Color& getFillColor() const;
	const Vector2& getMazeSize() const;
	const Vector2& getWallSize() const;
	const Rectangle& getFrame() const;
	const Texture2D* getTexture() const;

private:
	Vector2 mazeSize; Vector2 wallSize;
	Rectangle frame;
	Texture2D wallTexture;
};

#endif 