#include  "include/gtest/gtest.h"
#include "../Rendering Raylib/include/Tools.hpp"
#include "../Rendering Raylib/include/Map.hpp"
#include "../Rendering Raylib/include/Player.hpp"
#include "../Rendering Raylib/include/World.hpp"
#include "../Rendering Raylib/include/Weapon.hpp"
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
  EXPECT_FLOAT_EQ(getDist2Points(v1, v2), 7.2111025);
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
Player::Player(const Vector2& size, const Vector2& pos, const float angle, const Color& color, const std::string& texture) : 
    gun("../../Rendering Raylib/resources/guns/glock", 200, 20, 200, 10), color(color), sightDist(SIZE_PIXEL_MAP * 3), rotationAngle(angle),
    FOV(VIEW_ANGLE), circlePoints(COUNT_POINTS), segment(COUNT_POINTS), drawInfo()
{
    //texturePlayer = LoadTexture(texture.c_str());

    //healthTexture = LoadTexture("../../Rendering Raylib/resources/health.png");
	//font = LoadFontEx("../../Rendering Raylib/resources/Calibri.ttf", 30, nullptr, 0);
    bgHealth = {HEALTH_X, HEALTH_Y, (float)healthTexture.width + 2 * THICKNESS_FRAME, 
                (float)healthTexture.height + 2 * THICKNESS_FRAME};
    //soundInjury = LoadSound("../../Rendering Raylib/resources/injury.mp3");

    object.width = size.x; object.height = size.y;
    cameraPos.x = pos.x;
    cameraPos.y = pos.y; 
    object.x = pos.x - object.width / 2.0f;
    object.y = pos.y - object.height / 2.0f;
    mapShiftX = pos.x - THICKNESS_MAP * 2;
    mapShiftY = pos.y - THICKNESS_MAP * 2;

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

void Player::setId(int id)
{
    this->id = id;
}

// PLAYER TESTS
struct PlayerTest : public testing::Test {
  Player *pt;
  Map *mt;
  void SetUp() { mt = new Map("../../Rendering Raylib/resources/maze.txt"); 
                pt = new Player({mt->getWallSize().x / 4, mt->getWallSize().x / 4}, {3, 7}, 0, softRed, "../../Rendering Raylib/resources/player_1.png"); }
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
  EXPECT_FLOAT_EQ(pt->getSize().x, 2);
  EXPECT_FLOAT_EQ(pt->getSize().y, 2);
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

void World::removePlayer(int idPlayer)
{
    int idx = std::find(vecId.begin(), vecId.end(), idPlayer) - vecId.begin();
    //vecId.erase(vecId.begin() + idx);
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
  Player p = Player({5, 4}, {3, 7}, 0, softRed, "../../Rendering Raylib/resources/player_1.png");
  std::vector<Player> players;
  wt->addPlayer(p);
  players.push_back(p);
  EXPECT_EQ(players.size(), 1);
}

TEST_F(WorldTest, removePlayer)
{
  Player p = Player({5, 4}, {3, 7}, 0, softRed, "../../Rendering Raylib/resources/player_1.png");
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

// WEAPON FUNCTIONS
Weapon::Weapon(const std::string& gun, int cartridges, int oneClip, int maxCartridges, int damage) :
    actions(), cartridges(cartridges), oneClip(oneClip), maxCartridges(maxCartridges),
    curCartridge(oneClip), damage(damage)
{
    // //cartridgesTexture = LoadTexture("../../Rendering Raylib/resources/cartridges.png");
    // //redScope = LoadTexture("../../Rendering Raylib/resources/scope.png");
    // //font = LoadFontEx("../../Rendering Raylib/resources/Calibri.ttf", 30, nullptr, 0);
    // bgCarts = {CARTRIDGES_X, CARTRIDGES_Y, (float)cartridgesTexture.width + 2 * THICKNESS_FRAME, 
    //             (float)cartridgesTexture.height + 2 * THICKNESS_FRAME};
    // //soundShoot = LoadSound("../../Rendering Raylib/resources/shoot.mp3");
    // //soundReload = LoadSound("../../Rendering Raylib/resources/reload.mp3");
    // //soundEmpty = LoadSound("../../Rendering Raylib/resources/empty.mp3");

    // std::vector<std::string> files = {"/shootRight.png", "/shootLeft.png", "/reloadLeft.png", "/reloadRight.png"};
    // for (size_t i = 0; i < files.size(); ++i) {
    //     actions.push_back(LoadTexture((gun + files[i]).c_str()));
    //     animFrames.push_back({actions.back().width / WIDTH, actions.back().height / HEIGHT});
    // }
}
