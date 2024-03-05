#include "World.hpp"

World::World(const char* filename) : 
	gameMap(filename),
	player(gameMap), 
	miniMap(true)
{
	gameMap.findObjects();
	floor.width = GetRenderWidth();
	floor.height = GetRenderHeight() / 2.0f;
	floor.x = 0;
	floor.y = GetRenderHeight() / 2.0f;
}

void World::updateWorld(const float speed) 
{
	player.updatePosition(gameMap, speed);
	player.rotate(speed);
	player.camera.updateSegment(gameMap);
}

void World::showWorld() const
{
	DrawRectangleRec(floor, swamp);
	player.camera.show3DViewInWindow(gameMap);
	if (miniMap)
	{
		gameMap.showFrame();
		player.camera.show2DViewInWindow();
		player.show();
		gameMap.showObjectsInWindow();
	}
}

void World::setFlagMiniMap(bool flag)
{
	miniMap = flag;
}

bool World::getFlagMiniMap() const
{
	return miniMap;
}