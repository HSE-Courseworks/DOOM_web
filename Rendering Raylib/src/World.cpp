#include "World.hpp"

World::World(const std::string& _map, const std::string& _textures) : 
    gameMap(_map), players(), curIdPlayer(0)
{
    gameMap.findObjects();
    gameMap.readTextures(_textures);
    floor.width = GetRenderWidth();
    floor.height = GetRenderHeight() / 2.0f;
    floor.x = 0;
    floor.y = GetRenderHeight() / 2.0f;
}

void World::addPlayer(const Player& player)
{
    players.push_back(player);
}
void World::removePlayer(int idPlayer)
{
    players.erase(players.begin() + idPlayer);
}

void World::updateWorld(const float speed) 
{
    std::vector<Player*> opponents;
    //int next = 0;
    for (size_t i = 0; i < players.size(); ++i) {
        for (size_t j = 0; j < players.size(); ++j) {
            if (i == j) continue;
            opponents.push_back(&players[j]);
            //opponents[next++] = &players[j];
        }
        players[i].calculateRayDistances(gameMap, opponents);
        players[i].updateSegment();
        //next = 0;
        opponents.clear();
    }
    if (IsKeyReleased(KEY_V)) {
        curIdPlayer = (curIdPlayer + 1) % players.size();
    }
    players[curIdPlayer].updatePosition(gameMap, players, speed);
    players[curIdPlayer].rotate(speed);
    if (IsKeyReleased(KEY_M)) 
        players[curIdPlayer].setFlagMiniMap(!players[curIdPlayer].getFlagMiniMap());
}

void World::showWorld() const
{
    DrawRectangleRec(floor, grass);
    players[curIdPlayer].show3DViewInWindow();
    if (players[curIdPlayer].getFlagMiniMap())
    {
        gameMap.showFrame();
        for (const Player& player : players)
        {
            player.show2DViewInWindow();
            player.show();
        }
        gameMap.showObjectsInWindow();
    }
}