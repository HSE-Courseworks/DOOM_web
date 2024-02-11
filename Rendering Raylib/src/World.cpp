#include "World.hpp"

World::World(const char* filename) : 
	gameMap(filename),
	gameCamera(gameMap), 
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
	gameCamera.player.updatePosition(gameMap, speed);
	gameCamera.player.rotate(speed);
	gameCamera.updateSegment(gameMap);
}

void World::showWorld() const
{
	DrawRectangleRec(floor, swamp);
	gameCamera.show3DViewInWindow(gameMap);
	if (miniMap)
	{
		gameMap.showFrame();
		gameCamera.show2DViewInWindow();
		gameCamera.player.show();
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