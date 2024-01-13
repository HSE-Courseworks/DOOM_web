#pragma once

#define DEFAULT_VIEW_RADIUS (16)
#define DEFAULT_FOV (3.14159 / 4.0)
class Player
{
public:
	double x, y;
	double viewAngle;
	double FOV;
	int viewRadius;

	Player();
private:

};

