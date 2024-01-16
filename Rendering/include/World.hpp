#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "Map.hpp"
#include "Fps.hpp"

class World
{
public:
	bool miniMap;
	Map gameMap; Camera gameCamera; Fps curFps;
	World(RenderWindow& window, const std::string& filename);
	void showWorld(RenderWindow& window, float speed);

private:
	RectangleShape floor;
};

#endif 