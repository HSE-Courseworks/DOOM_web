#ifndef PLAYER_HPP
#define PLAYER_HPP

#define _USE_MATH_DEFINES
#include "raylib.h"
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <math.h>
#include <functional>
#include "Tools.hpp"
#include "Map.hpp"

#define SIDES (4)
#define DELTA (10e-4)
#define LIMIT (10e-3)
#define VIEW_ANGLE (60)
#define COUNT_POINTS (640)
#define REAL_HEIGHT (100)
#define DIST_SCREEN (COUNT_POINTS / (2 * tan(DegToRad(VIEW_ANGLE))))
#define KOEF_SPEED (25)
#define BLOCK (3)
#define SLOWDOWN (2)
#define TRANSPARENCY (5)
#define LOG_FONT_SIZE (25)
#define LOG_SHIFT_X (280)
#define LOG_SHIFT_Y (20)

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
// struct Comparator
// {
//     bool operator()(const DrawInfo3D& info1, const DrawInfo3D& info2) const {
//         float d1 = std::get<RAY_INFO::DIST>(info1);
//         float d2 = std::get<RAY_INFO::DIST>(info2);
//         float ray1 = std::get<RAY_INFO::NUM_RAY>(info1);
//         float ray2 = std::get<RAY_INFO::NUM_RAY>(info2);
//         return d1 < d2 || (d1 == d2 && ray1 < ray2);
//     }
// };

class Player
{
public:
    Player(const Vector2& size, const Vector2& pos, const float angle, 
           const Color& _color, const std::string& texture, const std::string& crosshair_path);
    void show() const;
    void updatePosition(const Map& gameMap, const std::vector<Player>& players, const float delta);
    void rotate(const float speed);
    float getRotation() const;
    const Vector2 getPosition() const;
    const Vector2 getSize() const;

    void drawCrosshair() const;
	void show3DViewInWindow() const; 
    void show2DViewInWindow() const;
    void showLog(const Font& fontLog) const;
    void calculateRayDistances(const Map& gameMap, const std::vector<Player*>& opponents);
    void updateSegment();
    void setFlagMiniMap(bool flag);
    bool getFlagMiniMap() const;
    void setFlagShowLog(bool flag);
    bool getFlagShowLog() const;
    void setId(int _id);
    int getId() const;

private:
    int id;
    Rectangle object; Color color;
    Texture2D texturePlayer;
    Texture2D crosshair;
    std::unordered_map<Vector2, float, Hash, Equal> mapDir;
    float sightDist, rotationAngle; 
    int FOV, circlePoints;
    std::vector<Vector2> segment;
    Vector2 cameraPos;
    std::vector<DrawInfo3D> drawInfo;
    bool miniMap = true;
    bool isLogEnabled = false;

    void detectCollision(const std::vector<const Rectangle*>& objects, Vector2& delta);
    Vector2 getCrossPoint(const int& numberSide, const std::vector<Vector2>& points, const float& k, const float& b) const;
    RayInfo getRayDistEnv(const Map& gameMap, const float angle, float& shiftX) const;
    float getRayDistPlayer(const Player* player, const float& k, const float& b, float& shiftX) const;
    RayInfo getIntersection(const Map& gameMap, Vector2& p, const Vector2& dp) const;
    std::unordered_set<int> getSeenPlayers(DrawInfo3D& centerObject) const;
    void calcRayDistEnv(const Map& gameMap);
    void calcRayDistPlayers(const std::vector<Player*>& opponents);
};

#endif 