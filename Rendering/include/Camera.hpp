#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Tools.hpp"
#include "Map.hpp"
#include "Player.hpp"

#define DELTA (10e-4)
#define LIMIT (10e-3)
#define VIEW_ANGLE (60)
#define COUNT_POINTS (720)
#define REAL_HEIGHT (100)
#define DIST_SCREEN (5 * COUNT_POINTS / (2 * tan(VIEW_ANGLE / 2)))

using namespace sf;

class Camera
{
public:
	Player player;	
	Camera(Map& gameMap);
	void show3DViewInWindow(RenderWindow& window, Map& gameMap);
	void show2DViewInWindow(RenderWindow& window);

private:
	float sightDist;
	int FOV, circlePoints;
	VertexArray segment;
	float calculateRayDist(Map& gameMap, float angle, float& shiftX);
	std::pair<float, Vector2i> getIntersection(Map& gameMap, Vector2f& p, Vector2f dp, Vector2f& cameraPos);
};

#endif 
