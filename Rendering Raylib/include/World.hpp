#ifndef WORLD_HPP
#define WORLD_HPP

//#define MAX_PLAYERS (5)

#include "raylib.h"
#include "Tools.hpp"
#include "Map.hpp"
#include "Player.hpp"

class World
{
public:
    Map gameMap; 

    World(const std::string& map, const std::string& textures);
    void addPlayer(const Player& player);
    void addPlayer(int id);
    void removePlayer(const int idPlayer);
    void updateWorld(const float speed);
    void showWorld() const;
    int GetCurPlayerID();
private:
    std::unordered_map<int, Player> players; // map: id -> player
    std::vector<int> vecId;
    int curPlayer, lastFreeId;
    Rectangle floor;
};

#endif 