#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "Map.hpp"
#include "Fps.hpp"

class World
{
public:
	Map gameMap; Camera gameCamera; Fps curFps;

	World(const RenderWindow& window, const std::string& filename);
	void updateWorld(RenderWindow& window, const float speed);
	void showWorld(RenderWindow& window) const;
	void setFlagMiniMap(bool flag);
	bool getFlagMiniMap() const;

private:
	bool miniMap;
	RectangleShape floor;
};

#endif 