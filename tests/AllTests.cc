#include  "include/gtest/gtest.h"
#include "../Rendering Raylib/include/Tools.hpp"
#include "../Rendering Raylib/include/Map.hpp"
#include "../Rendering Raylib/include/Player.hpp"
#include "../Rendering Raylib/include/World.hpp"
#include "../Rendering Raylib/include/Weapon.hpp"
#include "../Rendering Raylib/include/Timer.hpp"
#include "../Rendering Raylib/include/ScoreTable.hpp"
#include "../Rendering Raylib/include/PickUp.hpp"
#include "../Rendering Raylib/include/User.hpp"
#include "raylib.h"
#include <fstream>
#include <math.h>

#define _USE_MATH_DEFINES
#include <cmath>

// FPS FUNCTIONS
Fps::Fps()
{
	font = LoadFontEx("resources/Calibri.ttf", 30, nullptr, 0);
}

// TIMER FUNCTIONS
Timer::Timer(const int duration) : duration(duration), leftTime(duration) {
    backGround = {TIMER_X, TIMER_Y, TIMER_SIZE_X, TIMER_SIZE_Y};
    prevTime = 0;
}

int Timer::getLeftSeconds() const {
    return leftTime;
}

// SCORETABLE FUNCTIONS
ScoreTable::ScoreTable() : gameInfo() {
    inscriptions = {"Player", "K", "S", "D", "DMG", "KD"};
}

void ScoreTable::addPlayer(const int id, const std::string& nickName, const Color& color) {
    gameInfo[id] = std::make_pair(std::make_tuple(nickName, 0, 0, 0, 0, 0), color);
}

void ScoreTable::deletePlayer(const int id) {
    gameInfo.erase(id);
}

// PICKUP FUNTIONS
PickUp::PickUp(const char symbol, const Vector2& pos, Texture* texture)
    : symbol(symbol), isActive(true), timeGet(0), texture(texture), position(pos) {}

PickUp::PickUp(const PickUp& other) {
    symbol = other.symbol;
    position = other.position;
    isActive = other.isActive;
    timeGet = other.timeGet;
    texture = other.texture;
}

// MAP FUNCTIONS
Map::Map(const std::string& filename) : 
    scheme(), objects(), pickUps(COUNT_PICKUP_ALL), textures(), colors()
{
    frame.width = SIZE_PIXEL_MAP; frame.height = SIZE_PIXEL_MAP;
    frame.x = THICKNESS_MAP * 2; frame.y = THICKNESS_MAP * 2;

    std::ifstream file(filename, std::ios::in); std::string mazeLine;
    while (!file.eof()) 
    {
        file >> mazeLine;
        scheme.push_back(mazeLine);
    }
    file.close();
    mazeSize.x = scheme.front().size(); // Взять длину стенки миникарты
    mazeSize.y = scheme.size();         // Взять ширину стенки миникарты
    wallSize.x = WALL_SIZE;
    wallSize.y = WALL_SIZE;
}

void Map::readTextures(const std::string &filename) 
{
    std::ifstream file(filename, std::ios::in);
    std::string texture, type, colorR, colorG, colorB, colorA; 
    while (!file.eof()) 
    {
        file >> type >> texture >> colorR >> colorG >> colorB >> colorA;
        textures[type.front()] = LoadTexture(texture.data());
        unsigned char r = std::atoi(colorR.data()), g = std::atoi(colorG.data());
        unsigned char b = std::atoi(colorB.data()), a = std::atoi(colorA.data());
        colors[type.front()] = Color{r, g, b, a};
    }
    file.close();
}

void Map::findObjects()
{
    Rectangle wall = {0, 0, wallSize.x, wallSize.y};
    int N = scheme.size(), M = scheme.front().size();

    int posH = 0, posC = COUNT_PICKUP_CATEG, posA = COUNT_PICKUP_CATEG * 2;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            if (scheme[i][j] == 'H' || scheme[i][j] == 'C' || scheme[i][j] == 'A') {
                float posX = frame.x + j * wallSize.x + WALL_SIZE / 2;
                float posY = frame.y + i * wallSize.y + WALL_SIZE / 2;
                PickUp pickup(scheme[i][j], {posX, posY}, &textures[scheme[i][j]]);
                if (scheme[i][j] == 'H') pickUps[posH++] = pickup;
                else if (scheme[i][j] == 'C') pickUps[posC++] = pickup;
                else pickUps[posA++] = pickup;
            }
            else if (scheme[i][j] != '.')
            {
                wall.x = frame.x + j * wallSize.x;
                wall.y = frame.y + i * wallSize.y;
                objects.push_back({wall, scheme[i][j]});
            }
        }
    }
}

const Vector2& Map::getMazeSize() const
{
    return mazeSize;
}

const Vector2& Map::getWallSize() const
{
    return wallSize;
}

const Rectangle& Map::getFrame() const
{
    return frame;
}

// MAP TESTS
struct MapTest : public testing::Test {
  Map *mt;

  void SetUp() { mt = new Map("../../Rendering Raylib/resources/maze.txt"); }
  void TearDown() { delete mt; }
};

TEST_F(MapTest, getWallSize)
{
  Vector2 v1 = mt->getWallSize(), v2 = {4, 2};
  EXPECT_FLOAT_EQ(getDist2Points(v1, v2), 18.439089);
}

TEST_F(MapTest, getFrame)
{
  Rectangle r1 = mt->getFrame(), r2 = {20, 20};
  EXPECT_FLOAT_EQ(r1.x, r2.x);
  EXPECT_FLOAT_EQ(r1.y, r2.y);
}

TEST_F(MapTest, getMazeSize)
{
  Vector2 v1 = mt->getMazeSize(), v2 = {5, 7};
  EXPECT_FLOAT_EQ(getDist2Points(v1, v2), 33.970577);
}

TEST_F(MapTest, readTextures)
{
  std::ifstream file("../../Rendering Raylib/resources/maze.txt");
  mt->readTextures("../../Rendering Raylib/resources/maze.txt");
  file.close();
  EXPECT_EQ(file.is_open(), false);
}

// PLAYER FUNCTIONS
Player::Player(const Vector2 &pos, const float angle, const Color &color, const std::string &nickName) : 
    FOV(VIEW_ANGLE), circlePoints(COUNT_POINTS), sightDist(SIZE_PIXEL_MAP * 3), rotationAngle(angle), 
    nickName(nickName), color(color), drawInfo(), segment(COUNT_POINTS)
{
    backGroundH = {HEALTH_X, HEALTH_Y, BUFFS_SIZE + 2 * THICKNESS_FRAME, BUFFS_SIZE + 2 * THICKNESS_FRAME};
    backGroundA = {ARMOR_X, ARMOR_Y, BUFFS_SIZE + 2 * THICKNESS_FRAME, BUFFS_SIZE + 2 * THICKNESS_FRAME};

    cameraPos.x = pos.x;
    cameraPos.y = pos.y;
    mapShiftX = pos.x - THICKNESS_MAP * 2;
    mapShiftY = pos.y - THICKNESS_MAP * 2;

    mapDir[{0, 0}] = 0.0f;
    mapDir[{1, 0}] = 0.0f;
    mapDir[{1, 1}] = M_PI / 4;
    mapDir[{0, 1}] = M_PI / 2;
    mapDir[{-1, 1}] = 3 * M_PI / 4;
    mapDir[{-1, 0}] = M_PI;
    mapDir[{-1, -1}] = 5 * M_PI / 4;
    mapDir[{0, -1}] = 3 * M_PI / 2;
    mapDir[{1, -1}] = 7 * M_PI / 4;
}

float Player::getRotation() const
{
	return rotationAngle;
}

const Vector2& Player::getPosition() const { return cameraPos; }

Vector2 Player::getSize() const
{
    return {2 * RADIUS, 2 * RADIUS};
}

bool Player::getFlagMap() const
{
	return map;
}

bool Player::getFlagShowLog() const
{
    return isLogEnabled;
}

int Player::getId() const
{
    return id;
}

void Player::setId(int id)
{
    this->id = id;
}

// PLAYER TESTS
struct PlayerTest : public testing::Test {
  Player *pt;
  Map *mt;
  void SetUp() { mt = new Map("../../Rendering Raylib/resources/maze.txt"); 
                pt = new Player({5, 5}, 40, softRed, "Ivan"); }
  void TearDown() { delete mt; delete pt; }
};

TEST_F(PlayerTest, getRotation)
{
  EXPECT_FLOAT_EQ(pt->getRotation(), 40);
}

TEST_F(PlayerTest, getPosition)
{
  EXPECT_FLOAT_EQ(pt->getPosition().x, 5);
  EXPECT_FLOAT_EQ(pt->getPosition().y, 5);
}

TEST_F(PlayerTest, getSize)
{
  EXPECT_FLOAT_EQ(pt->getSize().x, 4);
  EXPECT_FLOAT_EQ(pt->getSize().y, 4);
}

TEST_F(PlayerTest, getFlagMap)
{
  EXPECT_EQ(pt->getFlagMap(), true);
}

TEST_F(PlayerTest, getFlagShowLog)
{
  EXPECT_EQ(pt->getFlagShowLog(), false);
}

TEST_F(PlayerTest, SetAndgetId)
{
  pt->setId(5);
  EXPECT_EQ(pt->getId(), 5);
}

// WORLD FUNCTIONS 
World::World(const std::string& map, const std::string& textures) : 
    gameMap(map), timer(120), scoreTable(), players(), fps()
{
    gameMap.readTextures(textures);
    gameMap.findObjects();
    floor.width = GetRenderWidth();
    floor.height = GetRenderHeight() / 2.0f;
    floor.x = 0;
    floor.y = GetRenderHeight() / 2.0f;

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

void World::addPlayer(const int id, const std::string& nickName)
{
    if (players.size() != MAX_PLAYERS && timer.getLeftSeconds() > 0)
    {
        Vector2 pos; float angle = 0; Color color;
        for (auto& [slot, idPlayer] : slots) {
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

void World::removePlayer(const int id)
{
    for (auto& [slot, idPlayer] : slots) {
        if (idPlayer == id) {
            idPlayer = -1;
            break;
        }
    }
    Player* playerForDelete = players[id].second;
    players[id].second = nullptr;
    players.erase(id);
    if (!timeEnd) scoreTable.deletePlayer(id);
    delete playerForDelete;
}

// WORLD TESTS
struct WorldTest : public testing::Test {
  World *wt;

  void SetUp() { wt = new World("../../Rendering Raylib/resources/maze.txt", "../../Rendering Raylib/resources/textures.txt"); }
  void TearDown() { delete wt; }
};

TEST_F(WorldTest, worldInit)
{
  std::vector<Player> players;
  EXPECT_EQ(players.size(), 0);
}

TEST_F(WorldTest, addPlayer)
{
  Player p = Player({5, 4}, 40, softRed, "Ivan");
  std::vector<Player> players;
  wt->addPlayer(0, "Ivan");
  players.push_back(p);
  EXPECT_EQ(players.size(), 1);
}

TEST_F(WorldTest, removePlayer)
{
  Player p = Player({5, 4}, 40, softRed, "Ivan");
  std::vector<Player> players;
  p.setId(5);
  wt->addPlayer(1, "Ivan");
  players.push_back(p);
  wt->removePlayer(1);
  players.pop_back();
  EXPECT_EQ(players.size(), 0);
}

// TOOLS FUNCTIONS
float DegToRad(const float angle)
{
    return angle * M_PI / 180;
}

float RadToDeg(const float angle)
{
    return angle * 180 / M_PI;
}

float getDist2Points(const Vector2& v1, const Vector2& v2)
{
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

Color changeLightness(const Color& color, const int delta)
{
    unsigned char r = std::max(color.r + 2 * delta / 6, 0);
    unsigned char g = std::max(color.g + 2 * delta / 6, 0);
    unsigned char b = std::max(color.b + 2 * delta / 6, 0);
    return {r, g, b, 255};
}

double constrainAngle360(const double angle) 
{
    return std::fmod(std::fmod(angle, 360.0) + 360.0, 360.0);
}

// TOOLS TESTS
TEST(ToolsTest, getDist2Points)
{
    Vector2 v1 = {4, 2}, v2 = {2, 10};
    EXPECT_FLOAT_EQ(getDist2Points(v1, v2), 8.24621);
}

TEST(ToolsTest, changeLightness)
{
    Color c = {55, 22, 13, 255}, c1 = {59, 26, 17, 255};
    c = changeLightness(c, 13);
    EXPECT_EQ(c.a, c1.a);
    EXPECT_EQ(c.b, c1.b);
    EXPECT_EQ(c.g, c1.g);
    EXPECT_EQ(c.r, c1.r);
}

TEST(ToolsTest, DegToRad)
{
    EXPECT_FLOAT_EQ(DegToRad(40), 0.69813168);
}

TEST(ToolsTest, RadToDeg)
{
    EXPECT_FLOAT_EQ(RadToDeg(40), 2291.8313);
}

TEST(ToolsTest, constrainAngle360)
{
    const double angle = 40;
    EXPECT_DOUBLE_EQ(constrainAngle360(angle), 40);
}

// WEAPON FUNCTIONS
Weapon::Weapon(const std::string& gun, int cartridges, int oneClip, int damage) :
    actions(), cartridges(cartridges), oneClip(oneClip), curCartridge(oneClip), damage(damage)
{ }

bool Weapon::checkShooting() const
{
    return isShooting;
}

bool Weapon::checkReloading() const
{
    return isReloading;
}

// WEAPON FUNCTIONS
struct WeaponTest : public testing::Test {
  Weapon *wt;

  void SetUp() { wt = new Weapon("glock", 20, 4, 5); }
  void TearDown() { delete wt; }
};

TEST_F(WeaponTest, checkShooting)
{
  EXPECT_EQ(wt->checkShooting(), false);
}

TEST_F(WeaponTest, checkReloading)
{
  EXPECT_EQ(wt->checkReloading(), false);
}

// USER FUNTIONS
User::User(const int id, const std::string& nickName) : id(id), nickName(nickName) {}

User::User(const User &other) : id(other.id), nickName(other.nickName) {}

int User::getId() const {
    return id;
}

std::string User::getNickName() const {
    return nickName;
}

bool User::getFlagActive() const {
    return active;
}

bool User::getFlagInGame() const {
    return inGame;
}

// USER TESTS
struct UserTest : public testing::Test {
  User *ut;

  void SetUp() { ut = new User(0, "Ivan"); }
  void TearDown() { delete ut; }
};

TEST_F(UserTest, getId)
{
  EXPECT_EQ(ut->getId(), 0);
}

TEST_F(UserTest, getNickName)
{
  EXPECT_EQ(ut->getNickName(), "Ivan");
}

TEST_F(UserTest, getFlagActive)
{
  EXPECT_EQ(ut->getFlagActive(), true);
}

TEST_F(UserTest, getFlagInGame)
{
  EXPECT_EQ(ut->getFlagInGame(), false);
}
