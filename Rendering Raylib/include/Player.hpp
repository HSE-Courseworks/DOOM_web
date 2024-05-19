#ifndef PLAYER_HPP
#define PLAYER_HPP

#define _USE_MATH_DEFINES
#include "raylib.h"
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <math.h>
#include <functional>
#include "Tools.hpp"
#include "Map.hpp"
#include "Weapon.hpp"

#define SIDES (4)
#define DELTA (10e-4)
#define LIMIT (10e-3)
#define VIEW_ANGLE (60)
#define COUNT_POINTS (640)
#define REAL_HEIGHT_WALL (100)
#define REAL_HEIGHT_PLAYER (75)
#define DIST_SCREEN (COUNT_POINTS / (2 * tan(DegToRad(VIEW_ANGLE))))
#define KOEF_SPEED (25)
#define BLOCK (3)
#define SLOWDOWN (2)
#define TRANSPARENCY (5)
#define RADIUS_SCOPE (8)
#define LOG_FONT_SIZE (25)
#define LOG_SHIFT_X (285)
#define LOG_SHIFT_Y (15)
#define LOG_FRAME_WIDTH (360)
#define LOG_FRAME_HEIGHT (270)
#define HEALTH_X (10)
#define HEALTH_Y (280)
#define INFO_HP_X (90)
#define INFO_HP_Y (295)

enum RAY_INFO { DIST, SHIFT_SPRITE, NUM_RAY, TEXTURE, SIZE_OBJ, ID };

using RayInfo = std::pair<float, Vector2>;
using DrawInfo3D = std::tuple<float, float, int, const Texture2D*, float, int>;

struct Hash
{
    std::size_t operator()(const Vector2& key) const
    {
        std::string hash = std::to_string(key.x) + std::to_string(key.y);
        return std::hash<std::string>()(hash);
    }
};
struct Equal
{
    bool operator()(const Vector2& lhs, const Vector2& rhs) const
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

class Player
{
public:
    Player() = default;
    Player(const Vector2& size, const Vector2& pos, const float angle, const Color& color, const std::string& texture);
    void show(const Vector2& shift) const;
    void updatePosition(const Map& gameMap, const std::unordered_map<int, Player>& players, const float delta);
    void rotate(const float speed);
    float getRotation() const;
    const Vector2 getPosition() const;
    const Vector2 getSize() const;


    void showScope() const;
	void show3DViewInWindow() const; 
    void show2DViewInWindow(const Vector2& shift) const;
    void showLog() const;
    void calculateRayDistances(const Map& gameMap, const std::vector<Player*>& opponents);
    void updateSegment();
    void setFlagMiniMap(bool flag);
    bool getFlagMiniMap() const;
    void setFlagShowLog(bool flag);
    bool getFlagShowLog() const;
    float getMapShiftX() const;
    float getMapShiftY() const;
    std::pair<float, int> getInfoCenterObject() const;
    const Weapon& getGun() const;
    Weapon& getGun();
    void takeDamage(int damage);
    void showHealth() const;
    void setId(int id);
    int getId() const;
    const std::unordered_set<int>& getDetectedEnemy() const;

private:
    int id, hp = 100;
    Weapon gun;
    DrawInfo3D centerObj;
    std::unordered_set<int> detectedEnemy;
    Rectangle object; Color color;
    Texture2D texturePlayer;
    std::unordered_map<Vector2, float, Hash, Equal> mapDir;
    float sightDist, rotationAngle;
    int FOV, circlePoints;
    std::vector<Vector2> segment;
    Vector2 cameraPos;
    float mapShiftX, mapShiftY;
    std::vector<DrawInfo3D> drawInfo;
    bool miniMap = true;
    bool isLogEnabled = false;
    Font font;
    Texture2D healthTexture;
    Rectangle bgHealth;
    Sound soundInjury;

    void detectCollision(const std::vector<const Rectangle*>& objects, Vector2& delta);
    Vector2 getCrossPoint(const int& numberSide, const std::vector<Vector2>& points, const float k, const float b) const;
    RayInfo getRayDistEnv(const Map& gameMap, const float angle, float& shiftX) const;
    float getRayDistPlayer(const Player* player, const float& k, const float& b, float& shiftX) const;
    RayInfo getIntersection(const Map& gameMap, Vector2& p, const Vector2& dp) const;
    void calcRayDistEnv(const Map& gameMap);
    void calcRayDistPlayers(const std::vector<Player*>& opponents);
};

#endif 