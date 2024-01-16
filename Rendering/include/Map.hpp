#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Tools.hpp"

#define SIZE_PIXEL_MAP (300)
#define THICKNESS_MAP (10)

using namespace sf;
using tableBool = std::vector<std::vector<bool>>;

class Map
{
public:
	tableBool scheme;
	std::vector<RectangleShape> objects;

	Map(const std::string& filename, const Color& color_sq);
	void findObjects();
	void showObjectsInWindow(RenderWindow& window);
	Color& getFillColor();
	Vector2f& getMazeSize();
	Vector2f& getWallSize();
	RectangleShape& getFrame();
	Texture* getTexture();

private:
	Vector2f mazeSize;
	Vector2f wallSize;
	Color color;
	RectangleShape frame;
	Texture wallTexture;
};

#endif 