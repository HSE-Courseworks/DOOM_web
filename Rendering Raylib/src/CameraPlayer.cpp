#include "CameraPlayer.hpp"
#include <iostream>

CameraPlayer::CameraPlayer(const Map& gameMap) : 
    player(gameMap.getWallSize().x / 4), 
    sightDist(SIZE_PIXEL_MAP),
    FOV(VIEW_ANGLE), circlePoints(COUNT_POINTS)
{
    segment = new Vector2[1 + circlePoints];

    float startX = gameMap.getFrame().x + gameMap.getWallSize().x * 3 / 2.0f;
    float startY = gameMap.getFrame().y + gameMap.getWallSize().y * 3 / 2.0f;
    player.setPosition({startX, startY});
}

std::pair<float, Vector2> CameraPlayer::getIntersection(const Map& gameMap, Vector2& p, const Vector2& dp, const Vector2& cameraPos) const
{
    float dist = sightDist; int total = gameMap.getMazeSize().x, mapX = 0, mapY = 0;
    for (int cnt = 0; cnt < total; ++cnt)
    {
        mapX = (p.y - gameMap.getFrame().y) / gameMap.getWallSize().y;
        mapY = (p.x - gameMap.getFrame().x) / gameMap.getWallSize().x;

        if (mapX >= 0 && mapX < gameMap.getMazeSize().x && mapY >= 0 &&
            mapY < gameMap.getMazeSize().y && gameMap.scheme[mapX][mapY])
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

float CameraPlayer::calculateRayDist(const Map& gameMap, const float angle, float& shiftX) const
{
    float pointX = 0, pointY = 0, x0 = 0, y0 = 0;
    Vector2 cameraPos = player.getPosition();

////////////////////////////////////////////////////////// Check HORIZONTAL lines //////////////////////////////////////////////////////////
    float ctgAngle = 1 / tan(angle); bool checkH = true;
    std::pair<float, Vector2> resultHor = { sightDist, {0, 0} };

    if (sin(angle) > LIMIT)                     // Looking DOWN
    {
        pointY = ((int)((cameraPos.y - gameMap.getFrame().y) / gameMap.getWallSize().y) + 1) *
                 gameMap.getWallSize().y + gameMap.getFrame().y;
        pointX = (pointY - cameraPos.y) * ctgAngle + cameraPos.x;
        y0 = gameMap.getWallSize().y;
        x0 = y0 * ctgAngle;
    }
    else if (sin(angle) < -LIMIT)               // Looking UP
    {
        pointY = (int)((cameraPos.y - gameMap.getFrame().y) / gameMap.getWallSize().y) *
                 gameMap.getWallSize().y + gameMap.getFrame().y - DELTA;
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

    Vector2 pointH = { pointX, pointY };
    if (checkH) resultHor = getIntersection(gameMap, pointH, {x0, y0}, cameraPos);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////// Check VERTICAL lines ////////////////////////////////////////////////////////////
    float tanAngle = tan(angle); bool checkV = true;
    std::pair<float, Vector2> resultVert = { sightDist, {0, 0} };

    if (cos(angle) < -LIMIT)                     // Looking LEFT
    {
        pointX = (int)((cameraPos.x - gameMap.getFrame().x) / gameMap.getWallSize().x) * 
                 gameMap.getWallSize().x + gameMap.getFrame().x - DELTA;
        pointY = (pointX - cameraPos.x) * tanAngle + cameraPos.y;
        x0 = -gameMap.getWallSize().x;
        y0 = x0 * tanAngle;
    }
    else if (cos(angle) > LIMIT)               // Looking RIGHT
    {
        pointX = ((int)((cameraPos.x - gameMap.getFrame().x) / gameMap.getWallSize().x) + 1) * 
                 gameMap.getWallSize().x + gameMap.getFrame().x;
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

    Vector2 pointV = { pointX, pointY };
    if (checkV) resultVert = getIntersection(gameMap, pointV, {x0, y0}, cameraPos);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (resultHor.first < resultVert.first)
        shiftX = pointH.x - gameMap.getFrame().x - resultHor.second.y * gameMap.getWallSize().x;
    else
        shiftX = (resultVert.second.x + 1) * gameMap.getWallSize().y - pointV.y + gameMap.getFrame().y;

    return std::min(resultHor.first, resultVert.first);
}

void CameraPlayer::show2DViewInWindow() const 
{
    for (int i = 0; i <= circlePoints; ++i) {
        DrawLineEx(player.getPosition(), segment[i], 1, semiTransparentWhite);
    }
}

void CameraPlayer::updateSegment(const Map& gameMap)
{
    const int FIRST = 0;
    segment[FIRST] = player.getPosition();
    float curAngle = player.getRotation() - FOV / 2;
    float deltaAngle = (float)FOV / circlePoints;

    for (int i = 0; i < circlePoints; ++i)
    {
        Vector2 curRayPoint;
        curRayPoint.x = cos(DegToRad(curAngle));
        curRayPoint.y = sin(DegToRad(curAngle));

        float shiftX = 0;
        float curDist = calculateRayDist(gameMap, DegToRad(curAngle), shiftX);

        curRayPoint.x = curRayPoint.x * curDist + player.getPosition().x;
        curRayPoint.y = curRayPoint.y * curDist + player.getPosition().y;
        segment[i + 1] = curRayPoint;
        curAngle += deltaAngle;
    }
}

void CameraPlayer::show3DViewInWindow(const Map& gameMap) const
{
    float curAngle = player.getRotation() - FOV / 2;
    float deltaAngle = (float)FOV / circlePoints;

    Rectangle wall; Rectangle crop;
    for (int i = 0; i < circlePoints; ++i)
    {
        float shiftX = 0;
        float curDist = calculateRayDist(gameMap, DegToRad(curAngle), shiftX);
        curDist *= cos(DegToRad(deltaAngle * i - FOV / 2));

        int widthWall = GetRenderWidth() / circlePoints;
        int heightWall = (int)(DIST_SCREEN / curDist * REAL_HEIGHT);

        wall.width = widthWall; wall.height = heightWall;
        wall.x = i * widthWall;
        wall.y = (GetRenderHeight() - heightWall) / 2;

        crop.x = (int)(gameMap.getTexture()->width * shiftX / gameMap.getWallSize().x);
        crop.y = 0;
        crop.width = (int)(gameMap.getTexture()->width / circlePoints);
        crop.height = gameMap.getTexture()->height;

        Color tint = changeLightness(lightGray, std::min(0.0f, -curDist));
        DrawTexturePro(*gameMap.getTexture(), crop, wall, {0, 0}, 0, tint);

        curAngle += deltaAngle;
    }
}
