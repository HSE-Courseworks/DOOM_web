#ifndef WORLD_HPP
#define WORLD_HPP

#include "raylib.h"
//#include "Server.hpp"
#include "client/Page.hpp"
#include "client/Tools.hpp"
#include "client/Map.hpp"
#include "client/Player.hpp"
#include "client/Timer.hpp"
#include "client/Fps.hpp"
#include "client/ScoreTable.hpp"
#include <unordered_map>
#include <queue>
#include <tuple>

#define MAX_DIST_TO_GET (40)

enum PlayerEvent {
    PLAYER_DISCONNECTED, NEW_PLAYER_CONNECTED,
    SHOOT, PICK_UP, RELOAD, 
    WLAK_FORWARD, WALK_BACK, WALK_LEFT, WALK_RIGHT
};

class World
{
public:
    Map gameMap;
    Timer timer;
    ScoreTable scoreTable;
    std::unordered_map<int, std::pair<bool, Player*>> players; // map: id -> player

    World(const std::string &map, const std::string &textures);
    void addPlayer(const int id, const std::string &nickName);
    void addPlayer(int id);
    void removePlayer(const int id);
    void resurrectPlayer(const int id);

    void compute();
    Pages update(const int id, const float speed);
    void show(const int id) const;
    void reboot();

    void pushEvent(PlayerEvent ev, int id);
    void setTimeEnd(const double time);
    int getPlayersNumber() const;
    double getTimeEnd() const;
    Player* getPlayer(int id);
    //int get
    friend sf::Packet& operator<<(sf::Packet& pack, World& world);
private:
    std::queue<std::tuple<PlayerEvent, int>> qEvents;
    Fps fps;
    Rectangle floor;
    std::vector<std::pair<std::tuple<Vector2, float, Color>, int>> slots;
    double timeEnd;

    void showMiniMap(const int id) const;
    void showMap(const int id) const;
};

sf::Packet& operator<<(sf::Packet& pack, World& world);

#endif