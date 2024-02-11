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
#define COUNT_POINTS (640)
#define REAL_HEIGHT (100)
#define DIST_SCREEN (4.5 * COUNT_POINTS / (2 * tan(VIEW_ANGLE / 2)))

using namespace sf;

class Camera
{
public:
	Player player;	
	Camera(const Map& gameMap);
	void show3DViewInWindow(RenderWindow& window, const Map& gameMap) const;
	void show2DViewInWindow(RenderWindow& window) const;
	void updateSegment(RenderWindow& window, const Map& gameMap);

private:
	float sightDist;
	int FOV, circlePoints;
	VertexArray segment;
	float calculateRayDist(const Map& gameMap, const float angle, float& shiftX) const;
	std::pair<float, Vector2i> getIntersection(const Map& gameMap, Vector2f& p, const Vector2f& dp, const Vector2f& cameraPos) const;
};

#endif 
