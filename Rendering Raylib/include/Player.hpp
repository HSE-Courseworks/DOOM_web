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
#include "ScoreTable.hpp"

#define SIDES (4)
#define TIME_SEEN (3)
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
#define RADIUS_SCOPE (8)
#define LOG_FONT_SIZE (25)
#define LOG_SHIFT_X (1560)
#define LOG_SHIFT_Y (50)
#define LOG_FRAME_WIDTH (360)
#define LOG_FRAME_HEIGHT (270)

#define BUFFS_SIZE (60)
#define HEALTH_X (10)
#define HEALTH_Y (840)
#define ARMOR_X (10)
#define ARMOR_Y (920)
#define INFO_ARMOR_X (90)
#define INFO_ARMOR_Y (935)
#define INFO_HP_X (90)
#define INFO_HP_Y (855)
#define INFO_NICK_X (10)
#define INFO_NICK_Y (280)

#define RADIUS (2.0f)
#define TEXTURE_SIZE (128)
#define COUNT_STAGES (5)
#define DELAY (10)

enum RAY_INFO { DIST, ROTATION, POS, SHIFT_SPRITE, NUM_RAY, TEXTURE, ID, STAGE_MOV };

using RayInfo = std::pair<float, Vector2>;
using DrawInfo3D = std::tuple<float, float, Vector2, float, int, const Texture2D*, int, int>;

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
    Player(const Vector2& pos, const float angle, const Color& color, const std::string& nickName);
    void show(const Vector2& shift) const;
    void updatePosition(const Map& gameMap, const std::vector<Player*>& players, const float delta);
    void rotate(const float speed);
    float getRotation() const;
    const Vector2 getPosition() const;
    void setRotation(float angle);
    void setPosition(Vector2& position);
    const Vector2 getSize() const;

    void showScope() const;
	void show3DViewInWindow() const; 
    void show2DViewInWindow(const Vector2& shift) const;
    void showLog() const;
    void calculateRayDistances(const Map& gameMap, const std::vector<Player*>& opponents);
    void updateSegment();
    void setFlagMap(bool flag);
    bool getFlagMap() const;
    void setFlagShowLog(bool flag);
    bool getFlagShowLog() const;
    void setFlagScoreTable(bool flag);
    bool getFlagScoreTable() const;
    float getMapShiftX() const;
    float getMapShiftY() const;
    void setLastTimeShoot(int time);
    int getLastTimeShoot() const;
    void updateHP(int cntHp);
    void updateArmor(int cntArmor);
    std::string getNickName() const;
    void showNickName() const;
    Color getColor() const;
    std::pair<float, int> getInfoCenterObject() const;
    const Weapon& getGun() const;
    Weapon& getGun();
    void takeDamage(int damage, int idOpp, ScoreTable& table);
    void showHealth() const;
    void showArmor() const;
    void setId(int id);
    int getId() const;
    int getHealth() const;
    void setHealth(const int newHp);
    void setArmor(const int newArmor);
    void setTimeDied(const double time);
    double getTimeDied() const;
    const std::unordered_set<int>& getDetectedEnemy() const;

private:
    int id, hp = 100, armor = 30;
    std::string nickName;
    Weapon gun;
    DrawInfo3D centerObj;
    std::unordered_set<int> detectedEnemy;
    Color color;
    Texture2D texturePlayer;
    std::unordered_map<Vector2, float, Hash, Equal> mapDir;
    float sightDist, rotationAngle;
    int FOV, circlePoints, lastTimeShoot = 0;
    std::vector<Vector2> segment;
    Vector2 cameraPos;
    float mapShiftX, mapShiftY;
    std::vector<DrawInfo3D> drawInfo;
    bool map = true, isLogEnabled = false, scoreTable = false;
    bool isMoving = false; int stageMoving = 0;
    Font font;
    Texture2D healthTexture, armorTexture;
    Rectangle backGroundH, backGroundA;
    Sound soundInjury, soundGetHp, soundGetArmor, soundHit;
    std::unordered_set<int> whoDmg;
    double whenDied = 0;

    void detectCollision(const std::vector<Rectangle>& objects, Vector2& delta);
    Vector2 getCrossPoint(const std::vector<Vector2>& points) const;
    RayInfo getRayDistEnv(const Map& gameMap, const float angle, float& shiftX) const;
    float getRayDistObject(const Vector2& position, const double k, const double b) const;
    RayInfo getIntersection(const Map& gameMap, Vector2& p, const Vector2& dp) const;
    void calcRayDistEnv(const Map& gameMap);
    void calcRayDistPlayers(const std::vector<Player*>& opponents);
    void calcRayDistPickUps(const std::vector<PickUp>& pickups);
    std::pair<float, float> calcAngleFOVObject(const float radius, const Vector2& position);
};

#endif 