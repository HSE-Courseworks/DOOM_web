#ifndef WORLD_HPP
#define WORLD_HPP

#include "raylib.h"
#include "Tools.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Timer.hpp"
#include "ScoreTable.hpp"

#define MAX_DIST_TO_GET (60)

class World
{
public:
    Map gameMap; Timer timer;
    ScoreTable scoreTable;

    World(const std::string& map, const std::string& textures);
    void addPlayer(const Player& player);
    void removePlayer(const int idPlayer);
    void updateWorld(const float speed);
    void showWorld() const;

private:
    std::unordered_map<int, Player> players; // map: id -> player
    std::vector<int> vecId;
    int curPlayer, lastFreeId;
    Rectangle floor;

    void showMiniMap() const;
    void showMap() const;
};

#endif 