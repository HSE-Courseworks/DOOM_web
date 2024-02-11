#include "Player.hpp"

Player::Player(const float size) : 
    rotationAngle(0), mapDir()
{
    object.width = size; object.height = size;
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

void Player::show() const
{
    DrawRectangleRec(object, BLACK);
    DrawRectangleLinesEx(object, 1, softRed);
}

void Player::detectCollision(const Map& gameMap, Vector2& delta)
{
    Rectangle copy(object); 
    copy.x += delta.x; copy.y += delta.y;

    std::vector<int> indexCollisions;
    for (size_t i = 0; i < gameMap.objects.size(); ++i)
    {
        if (CheckCollisionRecs(copy, gameMap.objects[i]))
            indexCollisions.push_back(i);
    }
    
    if (!indexCollisions.empty())
    {
        float delta_x = 0, delta_y = 0;
        for (const auto& ind : indexCollisions)
        {
            if (delta.x > 0)
                delta_x += copy.x + copy.width - gameMap.objects[ind].x;
            else
                delta_x += gameMap.objects[ind].x + gameMap.objects[ind].width - copy.x;

            if (delta.y > 0)
                delta_y += copy.y + copy.height - gameMap.objects[ind].y;
            else
                delta_y += gameMap.objects[ind].y + gameMap.objects[ind].height - copy.y;
        }

        if (fabs(delta_x - delta_y) < BLOCK) { delta.x = 0; delta.y = 0; }
        else if (delta_x > delta_y) delta.y = 0;
        else if (delta_y > delta_x) delta.x = 0;
    }
    object.x += delta.x;
    object.y += delta.y;
}

void Player::updatePosition(const Map& gameMap, const float delta)
{
    float rotAngle = DegToRad(rotationAngle), speed = delta * KOEF_SPEED;

    std::vector<std::pair<bool, Vector2>> dir;
    dir.push_back({ IsKeyDown(KEY_W), {1, 0} });
    dir.push_back({ IsKeyDown(KEY_A), {0, 1} });
    dir.push_back({ IsKeyDown(KEY_S), {-1, 0} });
    dir.push_back({ IsKeyDown(KEY_D), {0, -1} });

    Vector2 result = { 0, 0 };
    for (size_t i = 0; i < dir.size(); ++i)
    {
        Vector2 curVect = dir[i].second;
        curVect.x *= dir[i].first;
        curVect.y *= dir[i].first;
        result.x += curVect.x;
        result.y += curVect.y;
    }

    bool notStay = (result.x == 0 && result.y == 0) ? false : true;

    Vector2 deltaPos = { (float)(speed * cos(rotAngle - mapDir[result]) * notStay),
                          (float)(speed * sin(rotAngle - mapDir[result]) * notStay) };
    detectCollision(gameMap, deltaPos);
}

void Player::rotate(const float speed)
{
    int rotateDist = GetMousePosition().x - GetRenderWidth() / 2.0f;
    SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
    rotationAngle = rotationAngle + rotateDist * speed / SLOWDOWN;
}

float Player::getRotation() const
{
	return rotationAngle;
}

void Player::setPosition(const Vector2& pos)
{
    object.x = pos.x - object.width / 2.0f;
    object.y = pos.y - object.height / 2.0f;
}

const Vector2 Player::getPosition() const
{
    return {object.x + object.width / 2.0f, object.y + object.height / 2.0f};
}

const Vector2 Player::getSize() const
{
    return {object.width, object.width};
}