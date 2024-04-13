#include "Player.hpp"
#include <unordered_set>

Player::Player(const Vector2& size, const Vector2& pos, const float angle, const Color& _color, 
               const std::string& texture, const std::string& crosshair_path) : 
    color(_color), sightDist(SIZE_PIXEL_MAP), rotationAngle(angle),
    FOV(VIEW_ANGLE), circlePoints(COUNT_POINTS), segment(COUNT_POINTS), drawInfo()
{
    texturePlayer = LoadTexture(texture.data());
    Image crosshair_img = LoadImage(crosshair_path.data());
    ImageResize(&crosshair_img, crosshair_img.width / 10, crosshair_img.height / 10);
    crosshair = LoadTextureFromImage(crosshair_img);
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

void Player::drawCrosshair() const
{
    DrawTexture(crosshair, GetRenderWidth() / 2 - crosshair.width / 2, GetRenderHeight() / 2 - crosshair.height / 2, WHITE);
}

void Player::show() const
{
    DrawRectangleRec(object, BLACK);
    DrawRectangleLinesEx(object, 1, color);
}

void Player::detectCollision(const std::vector<const Rectangle*>& objects, Vector2& delta)
{
    Rectangle copy(object); 
    copy.x += delta.x; copy.y += delta.y;

    std::vector<int> indexCollisions;
    for (size_t i = 0; i < objects.size(); ++i)
    {
        if (CheckCollisionRecs(copy, *objects[i]))
            indexCollisions.push_back(i);
    }
    
    if (!indexCollisions.empty())
    {
        float delta_x = 0, delta_y = 0;
        for (const auto& ind : indexCollisions)
        {
            if (delta.x > 0)
                delta_x += copy.x + copy.width - objects[ind]->x;
            else
                delta_x += objects[ind]->x + objects[ind]->width - copy.x;

            if (delta.y > 0)
                delta_y += copy.y + copy.height - objects[ind]->y;
            else
                delta_y += objects[ind]->y + objects[ind]->height - copy.y;
        }

        if (fabs(delta_x - delta_y) < BLOCK) { delta.x = 0; delta.y = 0; }
        else if (delta_x > delta_y) delta.y = 0;
        else if (delta_y > delta_x) delta.x = 0;
    }
    object.x += delta.x;
    object.y += delta.y;
    cameraPos.x += delta.x;
    cameraPos.y += delta.y;
}

void Player::updatePosition(const Map& gameMap, const std::vector<Player>& players, const float delta)
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

    std::vector<const Rectangle*> objects;
    for (size_t i = 0; i < gameMap.objects.size(); ++i) {
        objects.push_back(&gameMap.objects[i].first);
    }
    for (size_t i = 0; i < players.size(); ++i) {
        if (&players[i] == this) continue;
        objects.push_back(&players[i].object);
    }
    detectCollision(objects, deltaPos);
}

void Player::rotate(const float speed)
{
    int rotateDist = GetMousePosition().x - GetRenderWidth() / 2.0f;
    SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
    rotationAngle += rotateDist * speed / SLOWDOWN;
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

void Player::setFlagMiniMap(bool flag)
{
	miniMap = flag;
}

bool Player::getFlagMiniMap() const
{
	return miniMap;
}

void Player::setFlagShowLog(bool flag)
{
    isLogEnabled = flag;
}

bool Player::getFlagShowLog() const
{
    return isLogEnabled;
}

void Player::setId(int _id)
{
    id = _id;
}
int Player::getId() const
{
    return id;
}

std::unordered_set<int> Player::getSeenPlayers(DrawInfo3D& centerObject) const  //получить id всех замеченных игроков
{
    std::unordered_set<int> seenPlayers;
    bool isChosen = false;
    for (const auto& objInfo : drawInfo)
    {
        int _id = std::get<RAY_INFO::ID>(objInfo);
        if (_id && !seenPlayers.contains(_id))
            seenPlayers.insert(_id);
        if (!isChosen && std::get<RAY_INFO::NUM_RAY>(objInfo) == 319)
        {
            centerObject = objInfo;
            isChosen = true;
        }
    }
    return seenPlayers;
}

void Player::showLog(const Font& fontLog) const
{
    // текущий игрок (номер)
    // текущая позиция игрока (x, y)
    // его угол обзора
    // направление обзора
    // замечен или не замечен противник

    DrawTextEx(fontLog, ("Current player: " + std::to_string(id)).c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y}, LOG_FONT_SIZE, 0, tintText);
    DrawTextEx(fontLog, ("Current position: (" + std::to_string(cameraPos.x) + ", " + std::to_string(cameraPos.y) + ")").c_str(), 
    {LOG_SHIFT_X, LOG_SHIFT_Y + LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);
    DrawTextEx(fontLog, ("Current angle of view: " + std::to_string(constrainAngle360(rotationAngle))).c_str(), 
    {LOG_SHIFT_X, LOG_SHIFT_Y + 2 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);
    DrawTextEx(fontLog, ("Current direction of view: " + std::to_string(int(constrainAngle360(rotationAngle)) / 90 + 1) + " quarter").c_str(), 
    {LOG_SHIFT_X, LOG_SHIFT_Y + 3 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);
    DrawTextEx(fontLog, ("Field of view: " + std::to_string(FOV)).c_str(), 
    {LOG_SHIFT_X, LOG_SHIFT_Y + 4 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);

    DrawInfo3D centerObject;
    std::unordered_set<int> seenPlayers = getSeenPlayers(centerObject);
    std::string strSeenPlayers;
    for (int _id : seenPlayers)
        strSeenPlayers += " " + std::to_string(_id);
    
    if (seenPlayers.empty())
        DrawTextEx(fontLog, "Detected players ID's: None", {LOG_SHIFT_X, LOG_SHIFT_Y + 5 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);
    else
        DrawTextEx(fontLog, ("Detected players ID's:" + strSeenPlayers).c_str(),
         {LOG_SHIFT_X, LOG_SHIFT_Y + 5 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);

    float distToObj; float sizeObj; int idObj;
    std::tie(distToObj, std::ignore, std::ignore, std::ignore, sizeObj, idObj) = centerObject;
    std::string obj = (idObj == 0 ? "WALL" : ("PLAYER, ID: " + std::to_string(idObj)));

    DrawTextEx(fontLog, ("Information about direct object:\n\n\t Seen: " + obj + 
        "\n\n\t Distance: " + std::to_string(distToObj) + "\n\n\t Object size: " + std::to_string(sizeObj)).c_str(), 
        {LOG_SHIFT_X, LOG_SHIFT_Y + 6 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);
    
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

RayInfo Player::getRayDistEnv(const Map& gameMap, const float angle, float& shiftX) const
{

    float pointX = 0, pointY = 0, x0 = 0, y0 = 0;
////////////////////////////////////////////////////////// Check HORIZONTAL lines //////////////////////////////////////////////////////////
    float ctgAngle = 1 / tan(angle); bool checkH = true;
    RayInfo resultHor = { sightDist, {0, 0} };

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
    if (checkH) resultHor = getIntersection(gameMap, pointH, {x0, y0});
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////// Check VERTICAL lines ////////////////////////////////////////////////////////////
    float tanAngle = tan(angle); bool checkV = true;
    RayInfo resultVert = { sightDist, {0, 0} };

    if (cos(angle) < -LIMIT)                     // Looking LEFT
    {
        pointX = (int)((cameraPos.x - gameMap.getFrame().x) / gameMap.getWallSize().x) * 
                 gameMap.getWallSize().x + gameMap.getFrame().x - DELTA;
        pointY = (pointX - cameraPos.x) * tanAngle + cameraPos.y;
        x0 = -gameMap.getWallSize().x;
        y0 = x0 * tanAngle;
    }
    else if (cos(angle) > LIMIT)                 // Looking RIGHT
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
    if (checkV) resultVert = getIntersection(gameMap, pointV, {x0, y0});
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2 mapCell;
    if (resultHor.first < resultVert.first) {
        mapCell.x = resultHor.second.x;
        mapCell.y = resultHor.second.y;
        shiftX = pointH.x - gameMap.getFrame().x - resultHor.second.y * gameMap.getWallSize().x;
    }
    else {
        mapCell.x = resultVert.second.x;
        mapCell.y = resultVert.second.y;
        shiftX = (resultVert.second.x + 1) * gameMap.getWallSize().y - pointV.y + gameMap.getFrame().y;
    }

    return std::make_pair(std::min(resultHor.first, resultVert.first), mapCell);
}

Vector2 Player::getCrossPoint(const int& numberSide, const std::vector<Vector2>& points, const float& k, const float& b) const 
{  
    Vector2 crossPoint = {1e10, 1e10};
    // Вертикальная стенка игрока  
    if (numberSide % 2) {
        if (std::fabs(k) > 1e8) return crossPoint;
        else if (std::fabs(k) < 1e-8) crossPoint = {points[numberSide].x, b};
        else crossPoint = {points[numberSide].x, k * points[numberSide].x + b};
    }
    // Горизонтальная стенка игрока
    else {
        if (std::fabs(k) < 1e-8) return crossPoint;
        else if (std::fabs(k) > 1e8) crossPoint = {b, points[numberSide].y};
        else crossPoint = {(points[numberSide].y - b) / k, points[numberSide].y};
    }
    return crossPoint;
}

float Player::getRayDistPlayer(const Player* player, const float& k, const float& b, float& shiftX) const
{
    std::vector<Vector2> points;
    const int eps = 1e-1, IS_VERTICAL = 2;
    points.push_back({player->object.x, player->object.y});
    points.push_back({player->object.x + player->object.width, player->object.y});
    points.push_back({player->object.x + player->object.width, player->object.y + player->object.height});
    points.push_back({player->object.x, player->object.y + player->object.height});

    float curDist = 0, resultDist = sightDist;
    float startAngle = constrainAngle360(rotationAngle - FOV / 2), endAngle = constrainAngle360(rotationAngle + FOV / 2);

    for (int curSide = 0; curSide < SIDES; ++curSide) {
        Vector2 crossPoint = getCrossPoint(curSide, points, k, b);
        float angle = constrainAngle360(RadToDeg(std::atan2(crossPoint.y - cameraPos.y, crossPoint.x - cameraPos.x)));
        bool check = true;

        float left = 0, right = 0, coord = 0;
        if (curSide % IS_VERTICAL) {
            left = points[(curSide % SIDES)].y;
            right = points[((curSide + 1) % SIDES)].y;
            coord = crossPoint.y;
        }
        else {
            left = points[(curSide % SIDES)].x;
            right = points[((curSide + 1) % SIDES)].x;
            coord = crossPoint.x;
        }

        float leftBound = std::min(left, right), rightBound = std::max(left, right);

        if (leftBound + eps < coord && rightBound - eps > coord) {
            curDist = getDist2Points(cameraPos, crossPoint);
            if (startAngle > endAngle) check = startAngle <= angle || angle <= endAngle;
            else check = startAngle <= angle && angle <= endAngle;
            if (curDist < resultDist && check) {
                resultDist = curDist;
                shiftX = crossPoint.y - leftBound;
            }
        }
    }
    return resultDist;
}

void Player::show2DViewInWindow() const 
{
    Color curColor = color; curColor.a = TRANSPARENCY;
    for (int i = 0; i <= circlePoints; ++i) {
        DrawLineEx(cameraPos, segment[i], 1, curColor);
    }
}

void Player::updateSegment()
{
    auto compar = [](const DrawInfo3D& info1, const DrawInfo3D& info2)
    { return std::get<RAY_INFO::DIST>(info1) < std::get<RAY_INFO::DIST>(info2); };
    std::sort(drawInfo.begin(), drawInfo.end(), compar);

    float deltaAngle = (float)FOV / circlePoints;
    std::unordered_set<int> setRays;
    for (auto iter = drawInfo.begin(); iter != drawInfo.end(); ++iter) 
    {
        int j = std::get<RAY_INFO::NUM_RAY>(*iter); 
        if (setRays.find(j) != setRays.end()) continue;

        setRays.insert(j);
        float curDist = std::get<RAY_INFO::DIST>(*iter) / cos(DegToRad(deltaAngle * j - FOV / 2));
        float curAngle = rotationAngle - FOV / 2 + j * deltaAngle;

        Vector2 curRayPoint;
        curRayPoint.x = cos(DegToRad(curAngle)) * curDist + cameraPos.x;
        curRayPoint.y = sin(DegToRad(curAngle)) * curDist + cameraPos.y;
        segment[j] = curRayPoint;
    }
}

void Player::calculateRayDistances(const Map& gameMap, const std::vector<Player*>& opponents) 
{
    drawInfo.clear();
    calcRayDistEnv(gameMap);
    calcRayDistPlayers(opponents);
}

void Player::show3DViewInWindow() const
{
    Rectangle wall; Rectangle crop;
    for (auto iter = drawInfo.rbegin(); iter != drawInfo.rend(); ++iter)
    {
        float curDist, shiftX, spriteSize; int j; const Texture2D* sprite; int isPlayer;
        std::tie(curDist, shiftX, j, sprite, spriteSize, isPlayer) = *iter;

        int widthWall = GetRenderWidth() / circlePoints;
        int realHeight = isPlayer ? REAL_HEIGHT / 2 : REAL_HEIGHT;
        int heightWall = (int)(DIST_SCREEN / curDist * realHeight);

        wall.width = widthWall; wall.height = heightWall;
        wall.x = j * widthWall;
        wall.y = isPlayer ? (GetRenderHeight() / 2) : (GetRenderHeight() - heightWall) / 2 ;

        crop.x = (int)(sprite->width * shiftX / spriteSize);
        crop.y = 0;
        crop.width = (int)(sprite->width / circlePoints);
        crop.height = sprite->height;

        Color tint = changeLightness(lightGray, std::min(0.0f, -curDist));
        DrawTexturePro(*sprite, crop, wall, {0, 0}, 0, tint);
    }
}

void Player::calcRayDistPlayers(const std::vector<Player*>& opponents)
{
    float curAngle = rotationAngle - FOV / 2;
    float deltaAngle = (float)FOV / circlePoints;

    float sizePlayer = getSize().x;
    for (int i = 0; i < circlePoints; ++i)
    {
        float k = std::tan(DegToRad(curAngle)), b = 0;
        // Прямая вертикальная
        if (std::fabs(k) > 1e8) b = cameraPos.x;
        // Прямая горизонтальная
        else if (std::fabs(k) < 1e-8) b = cameraPos.y;
        // Прямая наклонная 
        else b = cameraPos.y - k * cameraPos.x;

        float curDist = sightDist, shiftX = 0; int chosen = -1;
        for (size_t j = 0; j < opponents.size(); ++j) {
            float curShiftX = 0;
            float rayDist = getRayDistPlayer(opponents[j], k, b, curShiftX);
            if (rayDist < curDist) {
                shiftX = curShiftX; curDist = rayDist; chosen = j;
            }
        }
        if (std::fabs(sightDist - curDist) > 1e-9) {
            curDist *= cos(DegToRad(deltaAngle * i - FOV / 2));
            drawInfo.push_back(std::make_tuple(curDist, shiftX, i, &opponents[chosen]->texturePlayer, sizePlayer, opponents[chosen]->id));
        }
        curAngle += deltaAngle;
    }
}

void Player::calcRayDistEnv(const Map& gameMap) 
{
    float curAngle = rotationAngle - FOV / 2;
    float deltaAngle = (float)FOV / circlePoints;

    float sizeWall = gameMap.getWallSize().x;
    for (int i = 0; i < circlePoints; ++i)
    {
        float shiftX = 0;
        RayInfo cur = getRayDistEnv(gameMap, DegToRad(curAngle), shiftX);
        cur.first *= cos(DegToRad(deltaAngle * i - FOV / 2));
        char type = gameMap.scheme[cur.second.x][cur.second.y];

        drawInfo.push_back(std::make_tuple(cur.first, shiftX, i, gameMap.getTexture(type), sizeWall, 0));
        curAngle += deltaAngle;
    }
}