#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "raylib.h"
#include <cmath>
#include "Tools.hpp"
#include "Map.hpp"

#define DELTA (10e-4)
#define LIMIT (10e-3)
#define VIEW_ANGLE (60)
#define COUNT_POINTS (640)
#define REAL_HEIGHT (100)
#define DIST_SCREEN (COUNT_POINTS / (2 * tan(DegToRad(VIEW_ANGLE))))

using rayInfo = std::pair<float, Vector2>;

class CameraPlayer
{
public:
	CameraPlayer();
	void show3DViewInWindow(const Map& gameMap) const;
	void show2DViewInWindow() const;
	void updateSegment(const Map& gameMap);

private:
	float sightDist, rotationAngle; 
	int FOV, circlePoints;
	Vector2* segment; Vector2 cameraPos;
	friend class Player;

	float calculateRayDist(const Map& gameMap, const float angle, float& shiftX) const;
	rayInfo getIntersection(const Map& gameMap, Vector2& p, const Vector2& dp) const;
};

#endif 
