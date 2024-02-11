#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Tools.hpp"

#define SIZE_PIXEL_MAP (240)
#define THICKNESS_MAP (10)

using namespace sf;
using tableBool = std::vector<std::vector<bool>>;

class Map
{
public:
	tableBool scheme;
	std::vector<RectangleShape> objects;

	Map(const std::string& filename);
	void findObjects();
	void showObjectsInWindow(RenderWindow& window) const;
	const Color& getFillColor() const;
	const Vector2f& getMazeSize() const;
	const Vector2f& getWallSize() const;
	const RectangleShape& getFrame() const;
	const Texture* getTexture() const;;

private:
	Vector2f mazeSize; Vector2f wallSize;
	Color color;
	RectangleShape frame;
	Texture wallTexture;
};

#endif 