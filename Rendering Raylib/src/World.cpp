#include "World.hpp"

World::World(const std::string& map, const std::string& textures) : 
    gameMap(map), timer(600), scoreTable(), players(), vecId(), curPlayer(-1), lastFreeId(1)
{
    gameMap.readTextures(textures);
    gameMap.findObjects();
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
        scoreTable.addPlayer(lastFreeId, player.getNickName(), player.getColor());
        vecId.push_back(lastFreeId++);
    }
}

void World::removePlayer(int idPlayer)
{
    int idx = std::find(vecId.begin(), vecId.end(), idPlayer) - vecId.begin();
    vecId.erase(vecId.begin() + idx);
    players.erase(idPlayer);
    scoreTable.deletePlayer(idPlayer);

    if (players.empty()) curPlayer = -1;
    if (curPlayer == idx)
        curPlayer = idx % vecId.size();
}

void World::updateWorld(const float speed) 
{
    if (curPlayer == -1) return;

    timer.update();
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
    Player* curPlayerObj = &players[vecId[curPlayer]];

    curPlayerObj->updatePosition(gameMap, players, speed); 
    curPlayerObj->rotate(speed);
    if (IsKeyReleased(KEY_L))
        curPlayerObj->setFlagShowLog(!curPlayerObj->getFlagShowLog());

    if ((!curPlayerObj->getFlagScoreTable() && IsKeyDown(KEY_M)) || IsKeyDown(KEY_Q)) curPlayerObj->setFlagMap(true);
    else curPlayerObj->setFlagMap(false);

    if ((!curPlayerObj->getFlagMap() && IsKeyDown(KEY_TAB)) || IsKeyDown(KEY_Q)) curPlayerObj->setFlagScoreTable(true);
    else curPlayerObj->setFlagScoreTable(false);

    if (curPlayerObj->getGun().checkShooting()) curPlayerObj->getGun().updateNextFrameShoot();
    if (curPlayerObj->getGun().checkReloading()) curPlayerObj->getGun().updateNextFrameReload();

    if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE))
        curPlayerObj->getGun().setFlagLeftHand(!curPlayerObj->getGun().getFlagLeftHand());

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        auto infoShoot = curPlayerObj->getInfoCenterObject();
        int damage = curPlayerObj->getGun().shoot(infoShoot);
        curPlayerObj->setLastTimeShoot(timer.getLeftSeconds());
        if (infoShoot.second > 0 && damage) players[infoShoot.second].takeDamage(damage, curPlayerObj->getId(), scoreTable);
    }
    if (IsKeyReleased(KEY_R)) curPlayerObj->getGun().reload();

    if (IsKeyReleased(KEY_E)) {
        auto [dist, idObj] = curPlayerObj->getInfoCenterObject();
        if (idObj >= 0 || dist >= MAX_DIST_TO_GET) return;

        if (idObj >= -COUNT_PICKUP_CATEG) 
            curPlayerObj->updateArmor(gameMap.pickUps[idObj + COUNT_PICKUP_ALL].getHowMuchAdd());
        else if (idObj >= -COUNT_PICKUP_CATEG * 2)
            curPlayerObj->getGun().updateAmmunition(gameMap.pickUps[idObj + COUNT_PICKUP_ALL].getHowMuchAdd());
        else
            curPlayerObj->updateHP(gameMap.pickUps[idObj + COUNT_PICKUP_ALL].getHowMuchAdd());    

        gameMap.pickUps[idObj + COUNT_PICKUP_ALL].setFlagActive(false);
        gameMap.pickUps[idObj + COUNT_PICKUP_ALL].setTimeGet(timer.getLeftSeconds());
    }

    for (auto& pickup : gameMap.pickUps) {
        if (!pickup.getFlagActive() && pickup.getTimeGet() - timer.getLeftSeconds() == TIME_REBIRTH)
            pickup.setFlagActive(true);
    }
}

void World::showMiniMap() const {
    int curIdPlayer = vecId[curPlayer];
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

    for (auto& pickup : gameMap.pickUps) {
        if (!pickup.getFlagActive()) continue;

        Vector2 posPick = pickup.getPosition();
        if (std::pow(curPlayerPos.x - posPick.x, 2) + std::pow(curPlayerPos.y - posPick.y, 2)
            <= std::pow(SIZE_PIXEL_MAP / 2, 2)) {
            pickup.show(mapPos);
        }
    }
}

void World::showMap() const {
    int curIdPlayer = vecId[curPlayer];

    if (!players.at(curIdPlayer).getFlagScoreTable())
        DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), softGray);
    Texture2D wholeMap = *gameMap.getMapImage();
    Vector2 posTexture = {(GetRenderWidth() - wholeMap.width) / 2.0f, (GetRenderHeight() - wholeMap.height) / 2.0f};
    DrawTexture(wholeMap, posTexture.x, posTexture.y, WHITE);
    DrawRectangleLinesEx({posTexture.x, posTexture.y, (float)wholeMap.width, (float)wholeMap.height}, THICKNESS_MAP, GRAY);

    Vector2 shift = {posTexture.x + SIZE_PIXEL_MAP / 2.0f - 2 * THICKNESS_MAP,
                     posTexture.y + SIZE_PIXEL_MAP / 2.0f - 2 * THICKNESS_MAP};
    players.at(curIdPlayer).show2DViewInWindow(shift);

    Vector2 curPlayerPos = players.at(curIdPlayer).getPosition();
    for (const auto& [id, player] : players)
    {
        if (id == curIdPlayer) { player.show(shift); continue; }

        int time = player.getLastTimeShoot();
        if (time && time - timer.getLeftSeconds() < TIME_SEEN) player.show(shift);

        Vector2 posOpp = player.getPosition();
        if (std::pow(curPlayerPos.x - posOpp.x, 2) + std::pow(curPlayerPos.y - posOpp.y, 2)
            <= std::pow(SIZE_PIXEL_MAP / 2, 2) && players.at(curIdPlayer).getDetectedEnemy().contains(id)) {
            player.show(shift);
        }
    }

    for (auto& pickup : gameMap.pickUps) {
        if (!pickup.getFlagActive()) continue;
        pickup.show(shift);
    }
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
    players.at(curIdPlayer).showArmor();
    players.at(curIdPlayer).showNickName();
    showMiniMap();

    if (players.at(curIdPlayer).getFlagShowLog())
        players.at(curIdPlayer).showLog();
    timer.show();
    if (players.at(curIdPlayer).getFlagScoreTable()) scoreTable.show();
    if (players.at(curIdPlayer).getFlagMap()) showMap();
}
