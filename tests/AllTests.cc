#include  "include/gtest/gtest.h"
#include "../Rendering Raylib/include/Tools.hpp"
#include "../Rendering Raylib/include/Map.hpp"
#include "../Rendering Raylib/include/Player.hpp"
#include "../Rendering Raylib/include/World.hpp"
#include "../Rendering Raylib/include/Weapon.hpp"
#include "../Rendering Raylib/include/Timer.hpp"
#include "../Rendering Raylib/include/ScoreTable.hpp"
#include "../Rendering Raylib/include/PickUp.hpp"
#include "raylib.h"
#include <fstream>


// TIMER FUNCTIONS
Timer::Timer(const int time) : time(time) {
    backGround = {TIMER_X, TIMER_Y, TIMER_SIZE_X, TIMER_SIZE_Y};
    prevTime = GetTime();
}

// SCORETABLE FUNCTIONS
ScoreTable::ScoreTable() : gameInfoPlayers() {
    inscriptions = {"Player", "K", "S", "D", "DMG", "KD"};
    frame = {SCORE_X, SCORE_Y, TABLE_SIZE_X, TABLE_SIZE_Y};
}

// PICKUP FUNTIONS
PickUp::PickUp(char symbol, const float radius, const Vector2& pos, Texture* texture)
    : symbol(symbol), radius(radius), isActive(true), timeGet(0), texture(texture), position(pos) {}

PickUp::PickUp(const PickUp& other) {
    symbol = other.symbol;
    radius = other.radius;
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
    const float radius = 2.0f;

    int posH = 0, posC = COUNT_PICKUP_CATEG, posA = COUNT_PICKUP_CATEG * 2;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            if (scheme[i][j] == 'H' || scheme[i][j] == 'C' || scheme[i][j] == 'A') {
                float posX = frame.x + j * wallSize.x + WALL_SIZE / 2;
                float posY = frame.y + i * wallSize.y + WALL_SIZE / 2;
                PickUp pickup(scheme[i][j], radius, {posX, posY}, &textures[scheme[i][j]]);
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
Player::Player(const Vector2& pos, const float angle, const Color& color, const std::string& texture, const std::string& nickName) : 
    nickName(nickName), color(color), sightDist(SIZE_PIXEL_MAP * 3), rotationAngle(angle),
    FOV(VIEW_ANGLE), circlePoints(COUNT_POINTS), segment(COUNT_POINTS), drawInfo()
{}

float Player::getRotation() const
{
	return rotationAngle;
}

const Vector2 Player::getPosition() const
{
    return cameraPos;
}

const Vector2 Player::getSize() const
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
                pt = new Player({mt->getWallSize().x / 4, mt->getWallSize().x / 4}, 40, softRed, "../../Rendering Raylib/resources/player_1.png", "Ivan"); }
  void TearDown() { delete mt; delete pt; }
};

TEST_F(PlayerTest, getRotation)
{
  EXPECT_FLOAT_EQ(pt->getRotation(), 40);
}

TEST_F(PlayerTest, getPosition)
{
  EXPECT_FLOAT_EQ(pt->getPosition().x, 0);
  EXPECT_FLOAT_EQ(pt->getPosition().y, 0);
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
        vecId.push_back(lastFreeId++);
    }
}


void World::removePlayer(int idPlayer)
{
    int idx = std::find(vecId.begin(), vecId.end(), idPlayer) - vecId.begin();
    players.erase(idPlayer);

    if (players.empty()) curPlayer = -1;
    if (curPlayer == idx)
        curPlayer = idx % vecId.size();
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
  Player p = Player({5, 4}, 40, softRed, "../../Rendering Raylib/resources/player_1.png", "Ivan");
  std::vector<Player> players;
  wt->addPlayer(p);
  players.push_back(p);
  EXPECT_EQ(players.size(), 1);
}

TEST_F(WorldTest, removePlayer)
{
  Player p = Player({5, 4}, 40, softRed, "../../Rendering Raylib/resources/player_1.png", "Ivan");
  std::vector<Player> players;
  p.setId(5);
  wt->addPlayer(p);
  players.push_back(p);
  wt->removePlayer(5);
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
{}
