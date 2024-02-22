#include "World.hpp"

World::World(const RenderWindow& window, const std::string& filename) : 
	gameMap(filename), gameCamera(gameMap), 
	floor(), miniMap(true), curFps(helvetica)
{
	gameMap.findObjects();
	floor.setSize(Vector2f(window.getSize().x, window.getSize().y / 2.0f));
	floor.setFillColor(swamp);
	floor.setPosition(Vector2f(0, window.getSize().y / 2.0f));
}

void World::updateWorld(RenderWindow& window, const float speed) 
{
	gameCamera.player.updatePosition(gameMap, speed);
	gameCamera.player.rotate(window, speed);
	gameCamera.updateSegment(window, gameMap);
}

void World::showWorld(RenderWindow& window) const
{
	window.draw(floor);
	gameCamera.show3DViewInWindow(window, gameMap);
	curFps.show(window);
	if (miniMap)
	{
		window.draw(gameMap.getFrame());
		gameCamera.show2DViewInWindow(window);
		gameCamera.player.show(window);
		gameMap.showObjectsInWindow(window);
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