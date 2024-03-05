#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raylib.h"
#include <vector>
#include <unordered_map>
#include "Tools.hpp"
#include "Map.hpp"
#include "CameraPlayer.hpp"

#define KOEF_SPEED (25)
#define BLOCK (3)
#define SLOWDOWN (3)

struct Hash
{
	std::size_t operator()(const Vector2& key) const
	{
		std::string hash = std::to_string(key.x) + std::to_string(key.y);
		return std::hash<std::string>()(hash);
	}
};
struct Equal
{
	bool operator()(const Vector2& lhs, const Vector2& rhs) const
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
};

class Player
{
public:
	CameraPlayer camera;	
	Player(const Map& gameMap);
	void show() const;
	void updatePosition(const Map& gameMap, const float delta);
	void rotate(const float speed);
	float getRotation() const;
	const Vector2 getPosition() const;
	const Vector2 getSize() const;

private:
	Rectangle object;
	std::unordered_map<Vector2, float, Hash, Equal> mapDir;
	void detectCollision(const Map& gameMap, Vector2& delta);
};

#endif 