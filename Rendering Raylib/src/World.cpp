#include "World.hpp"
#include "raylib.h"
#include "Tools.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Client.hpp"

World::World(const std::string& map, const std::string& textures) : 
    gameMap(map), players(), vecId(), curPlayer(-1), lastFreeId(1)
{
    gameMap.findObjects();
    gameMap.readTextures(textures);
    floor.width = GetRenderWidth();
    floor.height = GetRenderHeight() / 2.0f;
    floor.x = 0;
    floor.y = GetRenderHeight() / 2.0f;
}

void World::addPlayer(const Player& player)
{
    if (players.size() != MAX_PLAYERS)
    {
        if (curPlayer == -1) curPlayer = 0;

        players[lastFreeId] = player;
        players[lastFreeId].setId(lastFreeId);
        vecId.push_back(lastFreeId++);
    }
}

void World::addPlayer(int id) {
    players[id] = Player();
}

void World::removePlayer(int idPlayer)
{
    int idx = std::find(vecId.begin(), vecId.end(), idPlayer) - vecId.begin();
    vecId.erase(vecId.begin() + idx);
    players.erase(idPlayer);

    if (players.empty()) curPlayer = -1;
    if (curPlayer == idx)
        curPlayer = idx % vecId.size();
}

void World::updateWorld(const float speed) 
{
    if (curPlayer == -1) return;

    int curIdPlayer = vecId[curPlayer];
    std::vector<Player*> opponents(players.size() - 1);
    int next = 0;
    for (size_t i = 0; i < vecId.size(); ++i) {
        for (size_t j = 0; j < vecId.size(); ++j) {
            if (i == j) continue;
            opponents[next++] = &players[vecId[j]];
        }
        players[vecId[i]].calculateRayDistances(gameMap, opponents);
        players[vecId[i]].updateSegment();
        next = 0;
    }
    if (IsKeyReleased(KEY_V)) {
        curPlayer = (curPlayer + 1) % vecId.size(); 
        // Переключить игрока (нужно только для отладки, при мультиплеере можно убрать)
    }
    Player* curPlayerObj = &players[curIdPlayer];

    curPlayerObj->updatePosition(gameMap, players, speed); 
    curPlayerObj->rotate(speed);
    if (IsKeyReleased(KEY_M)) 
        curPlayerObj->setFlagMiniMap(!curPlayerObj->getFlagMiniMap());
    if (IsKeyReleased(KEY_L))
        curPlayerObj->setFlagShowLog(!curPlayerObj->getFlagShowLog());

    if (curPlayerObj->getGun().checkShooting()) curPlayerObj->getGun().updateNextFrameShoot();
    if (curPlayerObj->getGun().checkReloading()) curPlayerObj->getGun().updateNextFrameReload();

    if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE))
        curPlayerObj->getGun().setFlagLeftHand(!curPlayerObj->getGun().getFlagLeftHand());

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        auto infoShoot = curPlayerObj->getInfoCenterObject();
        int damage = curPlayerObj->getGun().shoot(infoShoot);
        if (infoShoot.second && damage) players[infoShoot.second].takeDamage(damage);
    }
    if (IsKeyReleased(KEY_R)) curPlayerObj->getGun().reload();
}

void World::showWorld() const
{
    if (curPlayer == -1) return;

    int curIdPlayer = vecId[curPlayer];
    DrawRectangleRec(floor, darkGray);
    players.at(curIdPlayer).show3DViewInWindow();
    players.at(curIdPlayer).showScope();
    players.at(curIdPlayer).getGun().showGun();
    players.at(curIdPlayer).getGun().showAmmunition();
    players.at(curIdPlayer).showHealth();
    if (players.at(curIdPlayer).getFlagMiniMap())
    {
        Vector2 mapPos = {THICKNESS_MAP * 2 + SIZE_PIXEL_MAP / 2.0f, THICKNESS_MAP * 2 + SIZE_PIXEL_MAP / 2.0f};
        Vector2 curPlayerPos = players.at(curIdPlayer).getPosition();
        mapPos.x -= curPlayerPos.x;
        mapPos.y -= curPlayerPos.y;
        gameMap.showFrame();
        gameMap.showObjectsInWindow(players.at(curIdPlayer).getMapShiftX(), players.at(curIdPlayer).getMapShiftY());
        players.at(curIdPlayer).show2DViewInWindow(mapPos);
        for (const auto& [id, player] : players)
        {
            if (id == curIdPlayer) {
                player.show(mapPos); continue;
            }

            Vector2 posOpp = player.getPosition();
            if (std::pow(curPlayerPos.x - posOpp.x, 2) + std::pow(curPlayerPos.y - posOpp.y, 2)
                <= std::pow(SIZE_PIXEL_MAP / 2, 2) && players.at(curIdPlayer).getDetectedEnemy().contains(id)) {
                player.show(mapPos);
            }
        }
    }
    if (players.at(curIdPlayer).getFlagShowLog())
        players.at(curIdPlayer).showLog();
}

int World::GetCurPlayerID() {
    return curPlayer;
}