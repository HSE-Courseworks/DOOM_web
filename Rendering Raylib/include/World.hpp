#ifndef WORLD_HPP
#define WORLD_HPP

#include "raylib.h"
#include "Page.hpp"
#include "Tools.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Timer.hpp"
#include "Fps.hpp"
#include "ScoreTable.hpp"

#define MAX_DIST_TO_GET (40)

class World
{
public:
    Map gameMap;
    Timer timer;
    ScoreTable scoreTable;
    std::unordered_map<int, std::pair<bool, Player*>> players; // map: id -> player

    World(const std::string &map, const std::string &textures);
    void addPlayer(const int id, const std::string &nickName);
    void removePlayer(const int id);
    void resurrectPlayer(const int id);

    Pages update(const int id, const float speed);
    void show(const int id) const;
    void reboot();

    void setTimeEnd(const double time);
    int getPlayersNumber() const;
    double getTimeEnd() const;

private:
    Fps fps;
    Rectangle floor;
    std::vector<std::pair<std::tuple<Vector2, float, Color>, int>> slots;
    double timeEnd;

    void showMiniMap(const int id) const;
    void showMap(const int id) const;
};

#endif