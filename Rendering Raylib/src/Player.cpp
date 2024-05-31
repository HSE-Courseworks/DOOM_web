#include "Player.hpp"
#include <fstream>

Player::Player(const Vector2& size, const Vector2& pos, const float angle, 
            const Color& color, const std::string& texture, const std::string& nickName) : 
    nickName(nickName), gun("resources/guns/glock", 200, 20, 200, 10), color(color), sightDist(SIZE_PIXEL_MAP * 3), rotationAngle(angle),
    FOV(VIEW_ANGLE), circlePoints(COUNT_POINTS), segment(COUNT_POINTS), drawInfo()
{
    texturePlayer = LoadTexture(texture.c_str());

    healthTexture = LoadTexture("resources/health.png");
    armorTexture = LoadTexture("resources/armor.png");
    font = LoadFontEx("resources/Calibri.ttf", 30, nullptr, 0);
    backGroundH = {HEALTH_X, HEALTH_Y, BUFFS_SIZE + 2 * THICKNESS_FRAME, BUFFS_SIZE + 2 * THICKNESS_FRAME};
    backGroundA = {ARMOR_X, ARMOR_Y, BUFFS_SIZE + 2 * THICKNESS_FRAME, BUFFS_SIZE + 2 * THICKNESS_FRAME};
    soundInjury = LoadSound("resources/injury.mp3");

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

void Player::showScope() const
{
    DrawCircle(GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f, RADIUS_SCOPE, darkGray);
    DrawCircle(GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f, RADIUS_SCOPE / 2.0f, RED);
}

void Player::show(const Vector2& shift) const
{
    Rectangle fake = object;
    fake.x += shift.x; fake.y += shift.y;
    DrawRectangleRec(fake, color);
    // DrawRectangleLinesEx(fake, 1, color);
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
    mapShiftX += delta.x;
    mapShiftY += delta.y;
}

void Player::updatePosition(const Map& gameMap, const std::unordered_map<int, Player>& players, const float delta)
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
    for (auto& [id, player] : players) {
        if (player.id == this->id) continue;
        objects.push_back(&player.object);
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

void Player::setFlagMap(bool flag)
{
	map = flag;
}

bool Player::getFlagMap() const
{
	return map;
}

void Player::setFlagShowLog(bool flag)
{
    isLogEnabled = flag;
}

bool Player::getFlagShowLog() const
{
    return isLogEnabled;
}

void Player::setFlagScoreTable(bool flag) {
    scoreTable = flag;
}

bool Player::getFlagScoreTable() const {
    return scoreTable;
}

void Player::setLastTimeShoot(int time) {
    lastTimeShoot = time;
}

int Player::getLastTimeShoot() const {
    return lastTimeShoot;
}

void Player::setId(int id)
{
    this->id = id;
}

int Player::getId() const
{
    return id;
}

float Player::getMapShiftX() const 
{
    return mapShiftX;
}

float Player::getMapShiftY() const
{
    return mapShiftY;
}

std::string Player::getNickName() const {
    return nickName;
}

void Player::setNickName(const std::string& newNickName) {
    nickName = newNickName;
}

void Player::showNickName() const {
	Vector2 bounds = MeasureTextEx(font, nickName.c_str(), 30, 0);
    Rectangle bgInfo = {INFO_NICK_X, INFO_NICK_Y, 3 * THICKNESS_FRAME + bounds.x, 3 * THICKNESS_FRAME + bounds.y};
    DrawRectangleRec(bgInfo, GRAY);
    DrawRectangleLinesEx(bgInfo, THICKNESS_FRAME, BLACK);
    DrawTextEx(font, nickName.c_str(), {INFO_NICK_X + 1.5 * THICKNESS_FRAME, INFO_NICK_Y + 2 * THICKNESS_FRAME}, 30, 0, BLACK);
}

Color Player::getColor() const {
    return color;
}

const Weapon& Player::getGun() const 
{
    return gun;
}

Weapon& Player::getGun()
{
    return gun;
}

std::pair<float, int> Player::getInfoCenterObject() const
{
    return std::make_pair(std::get<RAY_INFO::DIST>(centerObj), std::get<RAY_INFO::ID>(centerObj));
}

void Player::takeDamage(int damage, int idOpp, ScoreTable& table)
{
    int trueDmg = 0;
    if (armor) {
        trueDmg = std::min(damage, armor);
        armor -= trueDmg;
    }
    else {
        trueDmg = std::min(damage, hp);
        hp -= trueDmg;
    }
    SetSoundVolume(soundInjury, VOLUME);
    PlaySound(soundInjury);
    table.updateDamage(idOpp, trueDmg);
    whoDmg.insert(idOpp);
    if (!hp) 
    {
        table.updateKill(idOpp);
        table.updateDeath(id);
        for (auto& idPlayer : whoDmg) {
            if (idPlayer == idOpp) continue;
            table.updateSupport(idPlayer);
        }
        whoDmg.clear(); 

        // Дописать смерть игрока
    }
}

void Player::showHealth() const
{
    DrawRectangleRec(backGroundH, softRed);
    DrawRectangleLinesEx(backGroundH, THICKNESS_FRAME, BLACK);
    DrawTexture(healthTexture, HEALTH_X + THICKNESS_FRAME, HEALTH_Y + THICKNESS_FRAME, WHITE);

    const char *textInfo = TextFormat("%i", hp);
	Vector2 bounds = MeasureTextEx(font, textInfo, 30, 0);
    Rectangle bgInfo = {INFO_HP_X, INFO_HP_Y, 3 * THICKNESS_FRAME + bounds.x, 3 * THICKNESS_FRAME + bounds.y};
    DrawRectangleRec(bgInfo, GRAY);
    DrawRectangleLinesEx(bgInfo, THICKNESS_FRAME, BLACK);
    DrawTextEx(font, textInfo, {INFO_HP_X + 1.5 * THICKNESS_FRAME, INFO_HP_Y + 2 * THICKNESS_FRAME}, 30, 0, BLACK);
}

void Player::showArmor() const {
    DrawRectangleRec(backGroundA, darkBlue);
    DrawRectangleLinesEx(backGroundA, THICKNESS_FRAME, BLACK);
    DrawTexture(armorTexture, ARMOR_X + THICKNESS_FRAME, ARMOR_Y + THICKNESS_FRAME, WHITE);

    const char *textInfo = TextFormat("%i", armor);
	Vector2 bounds = MeasureTextEx(font, textInfo, 30, 0);
    Rectangle bgInfo = {INFO_ARMOR_X, INFO_ARMOR_Y, 3 * THICKNESS_FRAME + bounds.x, 3 * THICKNESS_FRAME + bounds.y};
    DrawRectangleRec(bgInfo, GRAY);
    DrawRectangleLinesEx(bgInfo, THICKNESS_FRAME, BLACK);
    DrawTextEx(font, textInfo, {INFO_ARMOR_X + 1.5 * THICKNESS_FRAME, INFO_ARMOR_Y + 2 * THICKNESS_FRAME}, 30, 0, BLACK);
}

const std::unordered_set<int>& Player::getDetectedEnemy() const
{
    return detectedEnemy;
}

void Player::showLog() const
{
    // Текущий игрок (идентификатор)
    // Текущая позиция игрока (x, y)
    // Угол обзора игрока
    // Направление обзора (четверть)
    // Замечен или не замечен противник

    std::stringstream stream;
    DrawRectangle(LOG_SHIFT_X - 5, LOG_SHIFT_Y - 5, LOG_FRAME_WIDTH, LOG_FRAME_HEIGHT, softGray);
    stream << "Current player ID: " << id;
    DrawTextEx(font, stream.str().c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y}, LOG_FONT_SIZE, 0, tintText);
    stream.str(std::string());
    stream << "Current position: (" << cameraPos.x - 2 * THICKNESS_MAP << ", " << cameraPos.y - 2 * THICKNESS_MAP << ")";
    DrawTextEx(font, stream.str().c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y + LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);
    stream.str(std::string());
    stream << "Current angle of view: " << constrainAngle360(rotationAngle);
    DrawTextEx(font, stream.str().c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y + 2 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);
    stream.str(std::string());
    stream << "Current direction of view: " << int(constrainAngle360(rotationAngle) / 90) + 1 << " quarter";
    DrawTextEx(font, stream.str().c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y + 3 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);
    stream.str(std::string());
    stream << "Field of view: " << FOV;
    DrawTextEx(font, stream.str().c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y + 4 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);

    std::string strSeenPlayers;
    for (int curId : detectedEnemy) strSeenPlayers += " " + std::to_string(curId);
    
    if (detectedEnemy.empty())
        DrawTextEx(font, "Detected players ID's: None", {LOG_SHIFT_X, LOG_SHIFT_Y + 5 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);
    else
        DrawTextEx(font, ("Detected players ID's:" + strSeenPlayers).c_str(),
         {LOG_SHIFT_X, LOG_SHIFT_Y + 5 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);

    float distToObj, sizeObj; int objId;
    std::tie(distToObj, std::ignore, std::ignore, std::ignore, sizeObj, objId) = centerObj;
    std::string obj = (objId == 0 ? "WALL" : ("PLAYER, ID: " + std::to_string(objId)));

    stream.str(std::string());
    stream << "Information about direct object:\n\n\t Seen: " << obj << "\n\n\t Distance: " 
           << distToObj << "\n\n\t Object size: " << sizeObj;
    DrawTextEx(font, stream.str().c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y + 6 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);
}

RayInfo Player::getIntersection(const Map& gameMap, Vector2& p, const Vector2& dp) const
{
    int total = gameMap.getMazeSize().x, mapX = 0, mapY = 0;
    for (int cnt = 0; cnt < total; ++cnt)
    {
        mapX = (p.y - gameMap.getFrame().y) / gameMap.getWallSize().y;
        mapY = (p.x - gameMap.getFrame().x) / gameMap.getWallSize().x;

        if (mapX >= 0 && mapX < gameMap.getMazeSize().x && mapY >= 0 &&
            mapY < gameMap.getMazeSize().y && gameMap.scheme[mapX][mapY] != '.')
        {
            return { getDist2Points(cameraPos, p), {(float)mapX, (float)mapY} };
            break;
        }
        p.x += dp.x;
        p.y += dp.y;
    }
    return { sightDist, {-1, -1} };
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

Vector2 Player::getCrossPoint(const int& numberSide, const std::vector<Vector2>& points, const float k, const float b) const 
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

void Player::show2DViewInWindow(const Vector2& shift) const 
{
    Color curColor = lightGray; curColor.a = TRANSPARENCY;
    Vector2 pos = { cameraPos.x + shift.x, cameraPos.y + shift.y };
    for (int i = 0; i < circlePoints; ++i) {
        Vector2 curPoint = segment[i];
        curPoint.x += shift.x;
        curPoint.y += shift.y;
        DrawLineEx(pos, curPoint, 1, curColor);
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
        int curId = std::get<RAY_INFO::ID>(*iter);
        if (setRays.find(j) != setRays.end()) continue;

        if (j == circlePoints / 2 - 1) centerObj = *iter;
        if (curId && !detectedEnemy.contains(curId)) detectedEnemy.insert(curId);

        setRays.insert(j);
        float curDist = std::get<RAY_INFO::DIST>(*iter) / cos(DegToRad(deltaAngle * j - FOV / 2));
        float curAngle = rotationAngle - FOV / 2 + j * deltaAngle;

        Vector2 curRayPoint;
        curDist = std::min(curDist, SIZE_PIXEL_MAP / 2.0f);
        curRayPoint.x = cos(DegToRad(curAngle)) * curDist + cameraPos.x;
        curRayPoint.y = sin(DegToRad(curAngle)) * curDist + cameraPos.y;
        segment[j] = curRayPoint;
    }
}

void Player::calculateRayDistances(const Map& gameMap, const std::vector<Player*>& opponents) 
{
    drawInfo.clear();
    detectedEnemy.clear();
    calcRayDistEnv(gameMap);
    calcRayDistPlayers(opponents);
}

void Player::show3DViewInWindow() const
{
    Rectangle wall; Rectangle crop;
    for (auto iter = drawInfo.rbegin(); iter != drawInfo.rend(); ++iter)
    {
        float curDist, shiftX, spriteSize; int j; const Texture2D* sprite; int curId;
        std::tie(curDist, shiftX, j, sprite, spriteSize, curId) = *iter;

        int widthWall = GetRenderWidth() / circlePoints;
        int realHeight = curId ? REAL_HEIGHT_PLAYER : REAL_HEIGHT_WALL;
        int heightWall = (int)(DIST_SCREEN / curDist * realHeight);

        wall.width = widthWall; wall.height = heightWall;
        wall.x = j * widthWall;
        wall.y = curId ? (GetRenderHeight() / 2.0f - heightWall / 3.0f) : (GetRenderHeight() - heightWall) / 2.0f;

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