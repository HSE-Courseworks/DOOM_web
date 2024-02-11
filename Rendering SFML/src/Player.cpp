#include "Player.hpp"

using namespace sf;

Player::Player(const float size, const Color& color) : 
    object(Vector2f(size, size)), mapDir(), rotationAngle(0)
{
    object.setOrigin(Vector2f(object.getSize().x / 2.0f, object.getSize().y / 2.0f));
    object.setFillColor(color);
    object.setOutlineColor(Color::Black);
    object.setOutlineThickness(1);

    mapDir[Vector2i(0, 0)] = 0.0f;
    mapDir[Vector2i(1, 0)] = 0.0f;
    mapDir[Vector2i(1, 1)] = M_PI / 4;
    mapDir[Vector2i(0, 1)] = M_PI / 2;
    mapDir[Vector2i(-1, 1)] = 3 * M_PI / 4;
    mapDir[Vector2i(-1, 0)] = M_PI;
    mapDir[Vector2i(-1, -1)] = 5 * M_PI / 4;
    mapDir[Vector2i(0, -1)] = 3 * M_PI / 2;
    mapDir[Vector2i(1, -1)] = 7 * M_PI / 4;
}

void Player::show(RenderWindow& window) const
{
    window.draw(object);
}

void Player::setPosition(const Vector2f& pos)
{
    object.setPosition(pos);
}

void Player::detectCollision(const Map& gameMap, Vector2f& delta)
{
    RectangleShape copy(object); copy.move(delta);

    float size = copy.getSize().x + copy.getSize().y;
    std::vector<int> indexCollisions;
    for (size_t i = 0; i < gameMap.objects.size(); ++i)
    {
        if (copy.getGlobalBounds().intersects(gameMap.objects[i].getGlobalBounds()))
            indexCollisions.push_back(i);
    }
    
    if (!indexCollisions.empty())
    {
        float delta_x = 0, delta_y = 0;
        for (const auto& ind : indexCollisions)
        {
            if (delta.x > 0)
                delta_x += copy.getGlobalBounds().left + copy.getGlobalBounds().width -
                           gameMap.objects[ind].getGlobalBounds().left;
            else
                delta_x += gameMap.objects[ind].getGlobalBounds().left + gameMap.objects[ind].getGlobalBounds().width -
                           copy.getGlobalBounds().left;

            if (delta.y > 0)
                delta_y += copy.getGlobalBounds().top + copy.getGlobalBounds().height -
                           gameMap.objects[ind].getGlobalBounds().top;
            else
                delta_y += gameMap.objects[ind].getGlobalBounds().top + gameMap.objects[ind].getGlobalBounds().height -
                           copy.getGlobalBounds().top;
        }

        if (fabs(delta_x - delta_y) < BLOCK) delta = Vector2f(0, 0);
        else if (delta_x > delta_y) delta.y = 0;
        else if (delta_y > delta_x) delta.x = 0;
    }
    object.move(delta);
}

void Player::updatePosition(const Map& gameMap, const float delta)
{
    float rotAngle = DegToRad(rotationAngle), speed = delta * KOEF_SPEED;

    std::vector<std::pair<bool, Vector2i>> dir;
    dir.push_back({ Keyboard::isKeyPressed(Keyboard::W), {1, 0} });
    dir.push_back({ Keyboard::isKeyPressed(Keyboard::A), {0, 1} });
    dir.push_back({ Keyboard::isKeyPressed(Keyboard::S), {-1, 0} });
    dir.push_back({ Keyboard::isKeyPressed(Keyboard::D), {0, -1} });

    Vector2i result = { 0, 0 };
    for (size_t i = 0; i < dir.size(); ++i)
    {
        Vector2i curVect = dir[i].second;
        curVect.x *= dir[i].first;
        curVect.y *= dir[i].first;
        result += curVect;
    }

    bool notStay = result == Vector2i{ 0, 0 } ? false : true;

    Vector2f deltaPos = { (float)(speed * cos(rotAngle - mapDir[result]) * notStay),
                          (float)(speed * sin(rotAngle - mapDir[result]) * notStay) };
    detectCollision(gameMap, deltaPos);
}

void Player::rotate(RenderWindow& window, const float speed)
{
    int rotateDist = Mouse::getPosition(window).x - window.getSize().x / 2.0f;
    Mouse::setPosition({ (int)(window.getSize().x / 2.0f), (int)(window.getSize().y / 2.0f) }, window);
    rotationAngle = rotationAngle + rotateDist * speed / 3;
}

float Player::getRotation() const
{
	return rotationAngle;
}

const Vector2f& Player::getPosition() const
{
	return object.getPosition();
}