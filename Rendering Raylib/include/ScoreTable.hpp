#ifndef SCORE_TABLE_HPP
#define SCORE_TABLE_HPP

#include "raylib.h"
#include "Tools.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

#define SCORE_X (1450)
#define SCORE_Y (550)
#define MARGIN (20)
#define THICKNESS_CEIL (1)
#define TABLE_SIZE_X (460)
#define TABLE_SIZE_Y (520)
#define CEIL_SIZE (20)

enum GAME_INFO
{
    NICKNAME,
    KILL,
    SUPPORT,
    DEATH,
    DAMAGE,
    KD
};

using GameInfo = std::tuple<std::string, int, int, int, int, float>;

class ScoreTable
{
public:
    ScoreTable();
    ScoreTable(const ScoreTable &other);
    ScoreTable &operator=(const ScoreTable &rht);
    void show(const Vector2 &position) const;
    void updateKill(const int id);
    void updateSupport(const int id);
    void updateDeath(const int id);
    void updateDamage(const int id, const int damage);
    void addPlayer(const int id, const std::string &nickName, const Color &color);
    void deletePlayer(const int id);
    void reboot();
    
    std::string getTopPlayer() const;
    std::vector<int> getOrderPlayers() const;

private:
    std::unordered_map<int, std::pair<GameInfo, Color>> gameInfo;
    std::vector<std::string> inscriptions;
    Font font;
};

#endif