#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include "Tools.hpp"
#include "Map.hpp"

#define KOEF_SPEED (25)
#define BLOCK (3)

using namespace sf;

struct Hash
{
	std::size_t operator()(const Vector2i& key) const
	{
		std::string hash = std::to_string(key.x) + std::to_string(key.y);
		return std::hash<std::string>()(hash);
	}
};
struct Equal
{
	bool operator()(const Vector2i& lhs, const Vector2i& rhs) const
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
};

class Player
{
public:	
	Player(const float radius, const Color& color);
	void show(RenderWindow& window) const;
	void setPosition(const Vector2f& pos);
	void updatePosition(const Map& gameMap, const float delta);
	void rotate(RenderWindow& window, const float speed);
	float getRotation() const;
	const Vector2f& getPosition() const;

private:
	float rotationAngle;
	RectangleShape object;
	std::unordered_map<Vector2i, float, Hash, Equal> mapDir;
	void detectCollision(const Map& gameMap, Vector2f& delta);
};

#endif 