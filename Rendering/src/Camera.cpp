#include "Camera.hpp"

using namespace sf;

Camera::Camera(Map& gameMap) : 
    player(gameMap.getWallSize().x / 4, softRed), 
    sightDist(SIZE_PIXEL_MAP),
    FOV(VIEW_ANGLE), circlePoints(COUNT_POINTS), segment(TriangleFan, 1 + circlePoints)
{
    float startX = gameMap.getFrame().getPosition().x + gameMap.getWallSize().x * 3 / 2;
    float startY = gameMap.getFrame().getPosition().y + gameMap.getWallSize().y * 3 / 2;
    player.setPosition(Vector2f(startX, startY));
    for (int i = 0; i < segment.getVertexCount(); ++i) segment[i].color = semiTransparentWhite;
}

std::pair<float, Vector2i> Camera::getIntersection(Map& gameMap, Vector2f& p, Vector2f dp, Vector2f& cameraPos)
{
    float dist = sightDist; int total = gameMap.getMazeSize().x, mapX = 0, mapY = 0;
    for (int cnt = 0; cnt < total; ++cnt)
    {
        mapX = (p.y - gameMap.getFrame().getPosition().y) / gameMap.getWallSize().y;
        mapY = (p.x - gameMap.getFrame().getPosition().x) / gameMap.getWallSize().x;

        if (mapX >= 0 && mapX < gameMap.getMazeSize().x && mapY >= 0 &&
            mapY < gameMap.getMazeSize().y && gameMap.scheme[mapX][mapY])
        {
            dist = getDist2Points(cameraPos, p);
            break;
        }
        p += dp;
    }
    return { dist, Vector2i(mapX, mapY) };
}

float Camera::calculateRayDist(Map& gameMap, float angle, float& shiftX)
{
    float pointX = 0, pointY = 0, x0 = 0, y0 = 0;
    Vector2f cameraPos = player.getPosition();

////////////////////////////////////////////////////////// Check HORIZONTAL lines //////////////////////////////////////////////////////////
    float ctgAngle = 1 / tan(angle); bool checkH = true;
    std::pair<float, Vector2i> resultHor = { sightDist, {0, 0} };

    if (sin(angle) > LIMIT)                     // Looking DOWN
    {
        pointY = ((int)((cameraPos.y - gameMap.getFrame().getPosition().y) / gameMap.getWallSize().y) + 1) *
                 gameMap.getWallSize().y + gameMap.getFrame().getPosition().y;
        pointX = (pointY - cameraPos.y) * ctgAngle + cameraPos.x;
        y0 = gameMap.getWallSize().y;
        x0 = y0 * ctgAngle;
    }
    else if (sin(angle) < -LIMIT)               // Looking UP
    {
        pointY = (int)((cameraPos.y - gameMap.getFrame().getPosition().y) / gameMap.getWallSize().y) *
                 gameMap.getWallSize().y + gameMap.getFrame().getPosition().y - DELTA;
        pointX = (pointY - cameraPos.y) * ctgAngle + cameraPos.x;
        y0 = -gameMap.getWallSize().y;
        x0 = y0 * ctgAngle;
    }
    else
    {
        pointX = cameraPos.x;
        pointY = cameraPos.y;
        checkH = false;
    }

    Vector2f pointH = { pointX, pointY };
    if (checkH) resultHor = getIntersection(gameMap, pointH, Vector2f(x0, y0), cameraPos);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////// Check VERTICAL lines ////////////////////////////////////////////////////////////
    float tanAngle = tan(angle); bool checkV = true;
    std::pair<float, Vector2i> resultVert = { sightDist, {0, 0} };

    if (cos(angle) < -LIMIT)                     // Looking LEFT
    {
        pointX = (int)((cameraPos.x - gameMap.getFrame().getPosition().x) / gameMap.getWallSize().x) * 
                 gameMap.getWallSize().x + gameMap.getFrame().getPosition().x - DELTA;
        pointY = (pointX - cameraPos.x) * tanAngle + cameraPos.y;
        x0 = -gameMap.getWallSize().x;
        y0 = x0 * tanAngle;
    }
    else if (cos(angle) > LIMIT)               // Looking RIGHT
    {
        pointX = ((int)((cameraPos.x - gameMap.getFrame().getPosition().x) / gameMap.getWallSize().x) + 1) * 
                 gameMap.getWallSize().x + gameMap.getFrame().getPosition().x;
        pointY = (pointX - cameraPos.x) * tanAngle + cameraPos.y;
        x0 = gameMap.getWallSize().x;
        y0 = x0 * tanAngle;
    }
    else
    {
        pointX = cameraPos.x;
        pointY = cameraPos.y;
        checkV = false;
    }

    Vector2f pointV = { pointX, pointY };
    if (checkV) resultVert = getIntersection(gameMap, pointV, Vector2f(x0, y0), cameraPos);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (resultHor.first < resultVert.first)
        shiftX = pointH.x - gameMap.getFrame().getPosition().x - resultHor.second.y * gameMap.getWallSize().x;
    else
        shiftX = (resultVert.second.x + 1) * gameMap.getWallSize().y - pointV.y + gameMap.getFrame().getPosition().y;

    return std::min(resultHor.first, resultVert.first);
}

void Camera::show2DViewInWindow(RenderWindow& window)
{
    window.draw(segment);
}

void Camera::show3DViewInWindow(RenderWindow& window, Map& gameMap)
{
    float curAngle = player.getRotation() - FOV / 2;
    float deltaAngle = (float)FOV / circlePoints;
    const int FIRST = 0;

    segment[FIRST].position = player.getPosition();
    RectangleShape rectWall;
    rectWall.setTexture(gameMap.getTexture());
    for (size_t i = 0; i < circlePoints; ++i)
    {
        Vector2f curRayPoint;
        curRayPoint.x = cos(DegToRad(curAngle));
        curRayPoint.y = sin(DegToRad(curAngle));

        float curViewAngle = DegToRad(deltaAngle * i - FOV / 2), shiftX = 0;
        float curDist = calculateRayDist(gameMap, DegToRad(curAngle), shiftX);

        curRayPoint = curRayPoint * curDist + player.getPosition();
        curDist *= cos(curViewAngle);

        int widthWall = window.getSize().x / circlePoints;
        int heightWall = std::min((int)(DIST_SCREEN / curDist * REAL_HEIGHT), (int)window.getSize().y);

        rectWall.setFillColor(changeLightness(lightGray, std::min(0.0f, -curDist)));
        rectWall.setSize(Vector2f(widthWall, heightWall));
        rectWall.setPosition(Vector2f(i * widthWall, (window.getSize().y - heightWall) / 2));

        Vector2i textureSize = (Vector2i)gameMap.getTexture()->getSize();
        rectWall.setTextureRect(Rect<int>({ (int)(textureSize.x * shiftX / gameMap.getWallSize().x), 0}, 
                                          { (int)(textureSize.x / circlePoints), textureSize.y}));

        window.draw(rectWall);

        curAngle += deltaAngle;
        segment[i + 1].position = curRayPoint;
    }
}
