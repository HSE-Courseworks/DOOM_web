#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Network.hpp>
#include <raylib.h>

// enum RAY_INFO { DIST, SHIFT_SPRITE, NUM_RAY, TEXTURE, SIZE_OBJ, ID };

// using RayInfo = std::pair<float, Vector2>;
// using DrawInfo3D = std::tuple<float, float, int, const Texture2D*, float, int>;

class Player {
public:
    void setId(int _id);
    int getId();
    friend sf::Packet& operator<<(sf::Packet& pack, Player& pl);
private:
    int id, hp = 100;
};

sf::Packet& operator<<(sf::Packet& pack, Player& pl);

#endif