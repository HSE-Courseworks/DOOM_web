#ifndef WORLD_HPP
#define WORLD_HPP

#include "raylib.h"
#include "Tools.hpp"
#include "Player.hpp"
#include "Map.hpp"

class World
{
public:
	Map gameMap; 
	Player player;
	World(const char* filename);
	void updateWorld(const float speed);
	void showWorld() const;
	void setFlagMiniMap(bool flag);
	bool getFlagMiniMap() const;

private:
	bool miniMap;
	Rectangle floor;
};

#endif 