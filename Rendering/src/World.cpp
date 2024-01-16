#include "World.hpp"

World::World(RenderWindow& window, const std::string& filename) : 
	gameMap(filename, swamp), gameCamera(gameMap), floor(), miniMap(true), curFps(helvetica)
{
	gameMap.findObjects();
	floor.setSize(Vector2f(window.getSize().x, window.getSize().y / 2));
	floor.setFillColor(swamp);
	floor.setPosition(Vector2f(0, window.getSize().y / 2));
}

void World::showWorld(RenderWindow& window, float speed)
{
	gameCamera.player.updatePosition(speed, gameMap);
	gameCamera.player.rotate(speed, window);

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
