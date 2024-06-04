#include  "include/gtest/gtest.h"
#include "../Rendering Raylib/include/Tools.hpp"
#include "../Rendering Raylib/include/Map.hpp"
#include "../Rendering Raylib/include/Player.hpp"
#include "../Rendering Raylib/include/World.hpp"
#include "raylib.h"
#include <fstream>

// MAP FUNCTIONS
Map::Map(const std::string& filename) : 
    scheme(), objects(), textures(), colors()
{
    std::cout << 1;
    frame.width = SIZE_PIXEL_MAP; frame.height = SIZE_PIXEL_MAP;
    frame.x = THICKNESS_MAP * 2; frame.y = THICKNESS_MAP * 2;

    std::ifstream file(filename, std::ios::in); std::string mazeLine;
    while (!file.eof()) 
    {
        file >> mazeLine;
        scheme.push_back(mazeLine);
    }
    file.close();
    mazeSize.x = scheme.front().size();
    mazeSize.y = scheme.size();      
    wallSize.x = SIZE_PIXEL_MAP / mazeSize.x;
    wallSize.y = SIZE_PIXEL_MAP / mazeSize.y;
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
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            if (scheme[i][j] != '.')
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
  EXPECT_FLOAT_EQ(getDist2Points(v1, v2), 12.806249);
}

TEST_F(MapTest, readTextures)
{
  std::ifstream file("../../Rendering Raylib/resources/maze.txt");
  mt->readTextures("../../Rendering Raylib/resources/maze.txt");
  file.close();
  EXPECT_EQ(file.is_open(), false);
}

// PLAYER FUNCTIONS
Player::Player(const Vector2& size, const Vector2& pos, const float angle, const Color& _color, 
               const std::string& texture, const std::string& crosshair_path) : 
    color(_color), sightDist(SIZE_PIXEL_MAP), rotationAngle(angle),
    FOV(VIEW_ANGLE), circlePoints(COUNT_POINTS), segment(COUNT_POINTS), drawInfo()
{
    object.width = size.x; object.height = size.y;
    object.x = pos.x; object.y = pos.y;
    cameraPos.x = object.x;
    cameraPos.y = object.y;
    object.x -= object.width / 2.0f;
    object.y -= object.height / 2.0f;


    mapDir[{0, 0}] = 0.0f; mapDir[{1, 0}] = 0.0f;
    mapDir[{1, 1}] = M_PI / 4; mapDir[{0, 1}] = M_PI / 2;
    mapDir[{-1, 1}] = 3 * M_PI / 4; mapDir[{-1, 0}] = M_PI;
    mapDir[{-1, -1}] = 5 * M_PI / 4; mapDir[{0, -1}] = 3 * M_PI / 2;
    mapDir[{1, -1}] = 7 * M_PI / 4;
}

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
    return {object.width, object.height};
}

bool Player::getFlagMiniMap() const
{
	return miniMap;
}

bool Player::getFlagShowLog() const
{
    return isLogEnabled;
}

int Player::getId() const
{
    return id;
}

RayInfo Player::getIntersection(const Map& gameMap, Vector2& p, const Vector2& dp) const
{
    float dist = sightDist; int total = gameMap.getMazeSize().x, mapX = 0, mapY = 0;
    for (int cnt = 0; cnt < total; ++cnt)
    {
        mapX = (p.y - gameMap.getFrame().y) / gameMap.getWallSize().y;
        mapY = (p.x - gameMap.getFrame().x) / gameMap.getWallSize().x;

        if (mapX >= 0 && mapX < gameMap.getMazeSize().x && mapY >= 0 &&
            mapY < gameMap.getMazeSize().y && gameMap.scheme[mapX][mapY] != '.')
        {
            dist = getDist2Points(cameraPos, p);
            break;
        }
        p.x += dp.x;
        p.y += dp.y;
    }
    Vector2 mapPos = { (float)mapX, (float)mapY };
    return { dist, mapPos };
}

void Player::setId(int _id)
{
    id = _id;
}

// PLAYER TESTS
struct PlayerTest : public testing::Test {
  Player *pt;
  Map *mt;
  void SetUp() { mt = new Map("../../Rendering Raylib/resources/maze.txt"); pt = new Player({mt->getWallSize().x / 4, mt->getWallSize().x / 4}, {3, 7}, 0, softRed, "../../Rendering Raylib/resources/player_1.png", "../../Rendering Raylib/resources/crosshair.png"); }
  void TearDown() { delete mt; delete pt; }
};

TEST_F(PlayerTest, getRotation)
{
  EXPECT_FLOAT_EQ(pt->getRotation(), 0);
}

TEST_F(PlayerTest, getPosition)
{
  EXPECT_FLOAT_EQ(pt->getPosition().x, 3);
  EXPECT_FLOAT_EQ(pt->getPosition().y, 7);
}

TEST_F(PlayerTest, getSize)
{
  EXPECT_FLOAT_EQ(pt->getSize().x, 4);
  EXPECT_FLOAT_EQ(pt->getSize().y, 4);
}

TEST_F(PlayerTest, getFlagMiniMap)
{
  EXPECT_EQ(pt->getFlagMiniMap(), true);
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
World::World(const std::string& _map, const std::string& _textures) : 
    gameMap(_map), players(), curIdPlayer(0)
{
    fontLog = LoadFont("resources/romulus.png");
    gameMap.findObjects();
    gameMap.readTextures(_textures);
    floor.width = GetRenderWidth();
    floor.height = GetRenderHeight() / 2.0f;
    floor.x = 0;
    floor.y = GetRenderHeight() / 2.0f;
    
    for (size_t i = 1; i <= MAX_PLAYERS; i++)
        free_slots.insert(i);
}

void World::addPlayer(const Player& player)
{
    if (!free_slots.empty())
    {
        players.push_back(player);
        players.back().setId(*free_slots.begin());
        free_slots.erase(free_slots.begin());
    }
}

void World::removePlayer(int idPlayer)
{
    free_slots.insert(players[idPlayer].getId());
    players.erase(players.begin() + idPlayer);
}

void World::updateWorld(const float speed) 
{
    std::vector<Player*> opponents;
    for (size_t i = 0; i < players.size(); ++i) {
        for (size_t j = 0; j < players.size(); ++j) {
            if (i == j) continue;
            opponents.push_back(&players[j]);
        }
        opponents.clear();
    }
}

// WORLD TESTS
struct WorldTest : public testing::Test {
  World *wt;

  void SetUp() { wt = new World("../../Rendering Raylib/resources/maze.txt", "../../Rendering Raylib/resources/textures.txt"); }
  void TearDown() { delete wt; }
};

TEST_F(WorldTest, worldInit)
{
  EXPECT_EQ(wt->players.size(), 0);
}

TEST_F(WorldTest, updateWorld)
{
  wt->updateWorld(5);
  Player p = Player({5, 4}, {3, 7}, 0, softRed, "../../Rendering Raylib/resources/player_1.png", "../../Rendering Raylib/resources/crosshair.png");
  wt->addPlayer(p);
  EXPECT_EQ(wt->players.size(), 1);
}

TEST_F(WorldTest, addPlayer)
{
  Player p = Player({5, 4}, {3, 7}, 0, softRed, "../../Rendering Raylib/resources/player_1.png", "../../Rendering Raylib/resources/crosshair.png");
  wt->addPlayer(p);
  EXPECT_EQ(wt->players.size(), 1);
}

TEST_F(WorldTest, removePlayer)
{
  Player p = Player({5, 4}, {3, 7}, 0, softRed, "../../Rendering Raylib/resources/player_1.png", "../../Rendering Raylib/resources/crosshair.png");
  wt->addPlayer(p);
  wt->removePlayer(0);
  EXPECT_EQ(wt->players.size(), 0);
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

float constrainAngle360(float angle) {
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
    EXPECT_FLOAT_EQ(constrainAngle360(40), 40);
}