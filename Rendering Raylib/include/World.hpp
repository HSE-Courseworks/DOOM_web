#ifndef WORLD_HPP
#define WORLD_HPP

#include "raylib.h"
#include "Tools.hpp"
#include "Map.hpp"
#include "Player.hpp"

class World
{
public:
    Map gameMap; 
    std::vector<Player> players;

    World(const std::string& _map, const std::string& _textures);
    void addPlayer(const Player& player);
    void removePlayer(int idPlayer);
    void updateWorld(const float speed);
    void showWorld() const;

private:
    int curIdPlayer;
    Rectangle floor;
};

#endif 