#include "World.hpp"
#include "raylib.h"
#include "Tools.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Client.hpp"

World::World(const std::string &map, const std::string &textures) : 
    gameMap(map), timer(300), scoreTable(), players(), fps(), tPrevWorld(std::chrono::steady_clock::now()), firstWorld(true)
{    
    MessageUDPsock.bind(ClientMessageUDPPort);
    gameMap.readTextures(textures);
    gameMap.findObjects();
    floor = {0, GetRenderHeight() / 2.0f,
             static_cast<float>(GetRenderWidth()), GetRenderHeight() / 2.0f};

    Vector2 pos_1 = {gameMap.getFrame().x + WALL_SIZE * 3 / 2.0f, gameMap.getFrame().y + WALL_SIZE * 3 / 2.0f};
    slots.push_back(std::make_pair(std::make_tuple(pos_1, 0, softRed), -1));

    Vector2 pos_2 = {gameMap.getFrame().x + WALL_SIZE * (gameMap.getMazeSize().x - 3 / 2.0f),
                     gameMap.getFrame().y + WALL_SIZE * (gameMap.getMazeSize().y - 3 / 2.0f)};
    slots.push_back(std::make_pair(std::make_tuple(pos_2, 180, softBlue), -1));

    Vector2 pos_3 = {gameMap.getFrame().x + WALL_SIZE * (gameMap.getMazeSize().x - 3 / 2.0f),
                     gameMap.getFrame().y + WALL_SIZE * 3 / 2.0f};
    slots.push_back(std::make_pair(std::make_tuple(pos_3, 180, softPink), -1));

    Vector2 pos_4 = {gameMap.getFrame().x + WALL_SIZE * 3 / 2.0f,
                     gameMap.getFrame().y + WALL_SIZE * (gameMap.getMazeSize().y - 3 / 2.0f)};
    slots.push_back(std::make_pair(std::make_tuple(pos_4, 0, softYellow), -1));
    timeEnd = 0;
}

void World::addPlayer(const int id, const std::string &nickName)
{
    if (players.size() != MAX_PLAYERS && timer.getLeftSeconds() > 0) {
        Vector2 pos; float angle = 0; Color color;
        for (auto &[slot, idPlayer] : slots) {
            if (idPlayer == -1) {
                std::tie(pos, angle, color) = slot;
                idPlayer = id;
                break;
            }
        }
        players[id] = {true, new Player(pos, angle, color, nickName)};
        players[id].second->setId(id);
        scoreTable.addPlayer(id, nickName, color);
    }
}

//void World::addPlayer(int id) {
//    players[id] = {true, new Player()};
//    players[id].second->setId(id);
//}

void World::removePlayer(const int id)
{
    for (auto &[slot, idPlayer] : slots) {
        if (idPlayer == id) {
            idPlayer = -1;
            break;
        }
    }
    Player *playerForDelete = players[id].second;
    players[id].second = nullptr;
    players.erase(id);
    if (!timeEnd) scoreTable.deletePlayer(id);
    delete playerForDelete;
}

void World::resurrectPlayer(const int id)
{
    players[id].first = true;
    players[id].second->setArmor(0);
    players[id].second->setHealth(100);
    players[id].second->getGun().setAmmunition(100, 20);

    Vector2 startPos;
    float angle = 0;
    for (auto &[slot, idPlayer] : slots) {
        if (idPlayer == id) {
            std::tie(startPos, angle, std::ignore) = slot;
            break;
        }
    }
    players[id].second->setRotation(angle);
    players[id].second->setPosition(startPos);
}

Pages World::update(const int id, const float speed)
{
    Player *curPlayerObj = players[id].second;

    if (timer.getLeftSeconds() > 0) timer.update();
    else { ShowCursor(); return Pages::SUMMARY; }

    if (!curPlayerObj->getHealth()) { HideCursor(); return Pages::RESURRECTION; }
    else players[id].first = true;

    if (IsKeyReleased(KEY_ESCAPE)) { ShowCursor(); return Pages::PAUSE; }

    std::vector<Player*> opponents;
    for (auto &[idPlayer, player] : players) {
        if (id == idPlayer || !player.first) continue;
        opponents.push_back(player.second);
    }

    curPlayerObj->calculateRayDistances(gameMap, opponents);
    curPlayerObj->updateSegment();
    curPlayerObj->updatePosition(gameMap, opponents, speed);
    curPlayerObj->rotate(speed);

    if (IsKeyReleased(KEY_L))
        curPlayerObj->setFlagShowLog(!curPlayerObj->getFlagShowLog());

    if ((!curPlayerObj->getFlagScoreTable() && IsKeyDown(KEY_M)) || IsKeyDown(KEY_Q))
        curPlayerObj->setFlagMap(true);
    else
        curPlayerObj->setFlagMap(false);

    if ((!curPlayerObj->getFlagMap() && IsKeyDown(KEY_TAB)) || IsKeyDown(KEY_Q))
        curPlayerObj->setFlagScoreTable(true);
    else
        curPlayerObj->setFlagScoreTable(false);

    if (curPlayerObj->getGun().checkShooting())
        curPlayerObj->getGun().updateNextFrameShoot();
    if (curPlayerObj->getGun().checkReloading())
        curPlayerObj->getGun().updateNextFrameReload();

    if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE))
        curPlayerObj->getGun().setFlagLeftHand(!curPlayerObj->getGun().getFlagLeftHand());

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        SendMessage(MessageUDPsock, SHOOT);
        auto infoShoot = curPlayerObj->getInfoCenterObject();
        int damage = curPlayerObj->getGun().shoot(infoShoot);
        curPlayerObj->setLastTimeShoot(timer.getLeftSeconds());
        if (infoShoot.second > 0 && damage) {
            players[infoShoot.second].second->takeDamage(damage, curPlayerObj->getId(), scoreTable);
            if (!players[infoShoot.second].second->getHealth()) {
                players[infoShoot.second].first = false;
                players[infoShoot.second].second->setTimeDied(GetTime());
            }
        }
    }

    if (IsKeyReleased(KEY_R)) {
        SendMessage(MessageUDPsock, RELOAD);
        curPlayerObj->getGun().reload();
    }

    if (IsKeyReleased(KEY_E)) {
        SendMessage(MessageUDPsock, PICK_UP);
        auto [dist, idObj] = curPlayerObj->getInfoCenterObject();
        if (idObj < 0 && dist < MAX_DIST_TO_GET) {
            if (idObj >= -COUNT_PICKUP_CATEG)
                curPlayerObj->updateArmor(gameMap.pickUps[idObj + COUNT_PICKUP_ALL].getHowMuchAdd());
            else if (idObj >= -COUNT_PICKUP_CATEG * 2)
                curPlayerObj->getGun().updateAmmunition(gameMap.pickUps[idObj + COUNT_PICKUP_ALL].getHowMuchAdd());
            else
                curPlayerObj->updateHP(gameMap.pickUps[idObj + COUNT_PICKUP_ALL].getHowMuchAdd());

            gameMap.pickUps[idObj + COUNT_PICKUP_ALL].setFlagActive(false);
            gameMap.pickUps[idObj + COUNT_PICKUP_ALL].setTimeGet(timer.getLeftSeconds());
        }
    }

    for (auto &pickup : gameMap.pickUps) {
        if (!pickup.getFlagActive() && pickup.getTimeGet() - timer.getLeftSeconds() == TIME_REBIRTH)
            pickup.setFlagActive(true);
    }

    return Pages::GAME;
}

void World::showMiniMap(const int id) const
{
    if (!players.at(id).first) return;

    Vector2 mapPos = {THICKNESS_MAP * 2 + SIZE_PIXEL_MAP / 2.0f, 
                        THICKNESS_MAP * 2 + SIZE_PIXEL_MAP / 2.0f};
    Vector2 curPlayerPos = players.at(id).second->getPosition();
    mapPos.x -= curPlayerPos.x;
    mapPos.y -= curPlayerPos.y;
    gameMap.showFrame();
    gameMap.showObjectsInWindow(players.at(id).second->getMapShiftX(), 
                                players.at(id).second->getMapShiftY());
    players.at(id).second->show2DViewInWindow(mapPos);
    for (const auto &[idPlayer, player] : players) {
        if (!player.first) continue;

        if (id == idPlayer) { player.second->show(mapPos); continue; }

        Vector2 posOpp = player.second->getPosition();
        if (std::pow(curPlayerPos.x - posOpp.x, 2) + 
            std::pow(curPlayerPos.y - posOpp.y, 2) <= std::pow(SIZE_PIXEL_MAP / 2, 2) && 
            players.at(id).second->getDetectedEnemy().contains(idPlayer)) {
            player.second->show(mapPos);
        }
    }

    for (auto &pickup : gameMap.pickUps) {
        if (!pickup.getFlagActive()) continue;

        Vector2 posPick = pickup.getPosition();
        if (std::pow(curPlayerPos.x - posPick.x, 2) + 
            std::pow(curPlayerPos.y - posPick.y, 2) <= std::pow(SIZE_PIXEL_MAP / 2, 2)) {
            pickup.show(mapPos);
        }
    }
}

void World::showMap(const int id) const
{
    if (!players.at(id).first) return;

    if (!players.at(id).second->getFlagScoreTable())
        DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), softGray);
    Texture2D wholeMap = *gameMap.getMapImage();
    Vector2 posTexture = {(GetRenderWidth() - wholeMap.width) / 2.0f, 
                            (GetRenderHeight() - wholeMap.height) / 2.0f};
    DrawTexture(wholeMap, posTexture.x, posTexture.y, WHITE);
    DrawRectangleLinesEx({posTexture.x, posTexture.y, static_cast<float>(wholeMap.width), 
                        static_cast<float>(wholeMap.height)}, THICKNESS_MAP, GRAY);

    Vector2 shift = {posTexture.x + SIZE_PIXEL_MAP / 2.0f - 2 * THICKNESS_MAP,
                     posTexture.y + SIZE_PIXEL_MAP / 2.0f - 2 * THICKNESS_MAP};
    players.at(id).second->show2DViewInWindow(shift);

    Vector2 curPlayerPos = players.at(id).second->getPosition();
    for (const auto &[idPlayer, player] : players) {
        if (!player.first) continue;

        if (id == idPlayer) { player.second->show(shift); continue; }

        int time = player.second->getLastTimeShoot();
        if (time && time - timer.getLeftSeconds() < TIME_SEEN)
            player.second->show(shift);

        Vector2 posOpp = player.second->getPosition();
        if (std::pow(curPlayerPos.x - posOpp.x, 2) + 
            std::pow(curPlayerPos.y - posOpp.y, 2) <= std::pow(SIZE_PIXEL_MAP / 2, 2) && 
            players.at(id).second->getDetectedEnemy().contains(idPlayer)) {
            player.second->show(shift);
        }
    }

    for (auto &pickup : gameMap.pickUps) {
        if (!pickup.getFlagActive()) continue;
        pickup.show(shift);
    }
}

void World::show(const int id) const
{
    if (!players.at(id).first) return;

    DrawRectangleRec(floor, darkGray);
    players.at(id).second->show3DViewInWindow();
    players.at(id).second->showScope();
    players.at(id).second->getGun().showGun();
    players.at(id).second->getGun().showAmmunition();
    players.at(id).second->showHealth();
    players.at(id).second->showArmor();
    players.at(id).second->showNickName();
    showMiniMap(id);

    if (players.at(id).second->getFlagShowLog())
        players.at(id).second->showLog();
    timer.show();
    if (players.at(id).second->getFlagScoreTable())
        scoreTable.show({SCORE_X, SCORE_Y});
    if (players.at(id).second->getFlagMap())
        showMap(id);

    fps.show();
}

void World::reboot()
{
    timeEnd = 0;
    timer.reboot();
    scoreTable.reboot();
}

void World::sendMessage(PlayerEvent ev) {
    SendMessage(MessageUDPsock, ev);
}

int World::getPlayersNumber() const { return players.size(); }

double World::getTimeEnd() const { return timeEnd; }

void World::setTimeEnd(const double time) { timeEnd = time; }

Player* World::GetPlayer(int id) { return players[id].second; }

sf::Packet& operator>>(sf::Packet& pack, World& world) 
{
    pack >> world.timer;
    bool isPlayers = false;
    pack >> isPlayers;
    while (isPlayers) 
    {       
        int id;        
        bool pairFirstParam;
        pack >> id >> pairFirstParam;
        if (!world.players.contains(id)) 
        {
            Vector2 pos; float angle = 0; Color color;
            for (auto &[slot, idPlayer] : world.slots) 
            {
                if (idPlayer == -1) {
                    std::tie(pos, angle, color) = slot;
                    idPlayer = id;
                    break;
                }
            }
            Player *pl = new Player(pos, angle, color, "");
            world.players[id] = std::make_pair(pairFirstParam, pl);
        } 
        else if (world.firstWorld || 
            std::chrono::duration_cast<std::chrono::milliseconds>(world.tPrevWorld - std::chrono::steady_clock::now()).count() >= 5000)
        {
            world.firstWorld = false;
            world.tPrevWorld = std::chrono::steady_clock::now();
            pack >> *world.players[id].second;
            world.players[id].first = pairFirstParam;
        }
        else 
        {
            Player pl;
            pack >> pl;
        }
        pack >> isPlayers;
    }
    //for (auto& sl : world.slots) {
    //    pack >> std::get<0>(sl.first).x >> std::get<0>(sl.first).y >> std::get<1>(sl.first) >> sl.second;
    //}
    pack >> world.timeEnd;
    return pack;
}