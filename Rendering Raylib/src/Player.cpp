#include "Player.hpp"

Player::Player(const Vector2 &pos, const float angle, const Color &color, const std::string &nickName) : 
    FOV(VIEW_ANGLE), circlePoints(COUNT_POINTS), sightDist(SIZE_PIXEL_MAP * 3), rotationAngle(angle),
    gun("resources/guns/glock", 100, 20, 10), nickName(nickName), color(color), drawInfo(), segment(COUNT_POINTS)
{
    font = LoadFontEx("resources/Calibri.ttf", FONT_SIZE_INFO, nullptr, 0);
    texturePlayer = LoadTexture("resources/player.png");
    healthTexture = LoadTexture("resources/health.png");
    armorTexture = LoadTexture("resources/armor.png");
    backGroundH = {HEALTH_X, HEALTH_Y, BUFFS_SIZE + 2 * THICKNESS_FRAME, BUFFS_SIZE + 2 * THICKNESS_FRAME};
    backGroundA = {ARMOR_X, ARMOR_Y, BUFFS_SIZE + 2 * THICKNESS_FRAME, BUFFS_SIZE + 2 * THICKNESS_FRAME};
    soundInjury = LoadSound("resources/injury.mp3");
    soundGetHp = LoadSound("resources/getHP.mp3");
    soundGetArmor = LoadSound("resources/getArmor.mp3");

    cameraPos.x = pos.x;
    cameraPos.y = pos.y;
    mapShiftX = pos.x - THICKNESS_MAP * 2;
    mapShiftY = pos.y - THICKNESS_MAP * 2;

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

void Player::updatePosition(const Map &gameMap, const std::vector<Player*> &players, const float delta)
{
    float rotAngle = DegToRad(rotationAngle), speed = delta * KOEF_SPEED;
    std::vector<std::pair<bool, Vector2>> dir;

    dir.push_back({IsKeyDown(KEY_W), {1, 0}});
    dir.push_back({IsKeyDown(KEY_A), {0, 1}});
    dir.push_back({IsKeyDown(KEY_S), {-1, 0}});
    dir.push_back({IsKeyDown(KEY_D), {0, -1}});

    Vector2 result = {0, 0};
    for (size_t i = 0; i < dir.size(); ++i) {
        Vector2 curVect = dir[i].second;
        curVect.x *= dir[i].first;
        curVect.y *= dir[i].first;
        result.x += curVect.x;
        result.y += curVect.y;
    }

    bool notStay = (result.x == 0 && result.y == 0) ? false : true;
    Vector2 deltaPos = {(float)(speed * cos(rotAngle - mapDir[result]) * notStay),
                        (float)(speed * sin(rotAngle - mapDir[result]) * notStay)};

    if (!notStay) { isMoving = false; stageMoving = 0; return; }

    std::vector<Rectangle> objects;
    for (size_t i = 0; i < gameMap.objects.size(); ++i) {
        objects.push_back(gameMap.objects[i].first);
    }
    for (auto &player : players) {
        objects.push_back({player->getPosition().x - RADIUS, player->getPosition().y - RADIUS, 
                            2 * RADIUS, 2 * RADIUS});
    }
    detectCollision(objects, deltaPos);
}

void Player::calculateRayDistances(const Map &gameMap, const std::vector<Player*> &opponents)
{
    drawInfo.clear();
    detectedEnemy.clear();
    calcRayDistEnv(gameMap);
    calcRayDistPlayers(opponents);
    calcRayDistPickUps(gameMap.pickUps);
}

void Player::updateSegment()
{
    auto compar = [](const DrawInfo3D &info1, const DrawInfo3D &info2)
    { return std::get<RAY_INFO::DIST>(info1) < std::get<RAY_INFO::DIST>(info2); };
    std::sort(drawInfo.begin(), drawInfo.end(), compar);

    float deltaAngle = (float)FOV / circlePoints;
    std::unordered_set<int> setRays;

    for (auto iter = drawInfo.begin(); iter != drawInfo.end(); ++iter) {
        int j = std::get<RAY_INFO::NUM_RAY>(*iter);
        int curId = std::get<RAY_INFO::ID>(*iter);
        if (setRays.find(j) != setRays.end()) continue;

        if (j == circlePoints / 2 - 1) centerObj = *iter;
        if (curId > 0 && !detectedEnemy.contains(curId)) detectedEnemy.insert(curId);

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

void Player::rotate(const float speed)
{
    int rotateDist = GetMousePosition().x - GetRenderWidth() / 2.0f;
    SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
    rotationAngle += rotateDist * speed * KOEF_ROTATE;
}

void Player::updateHP(const int cntHp)
{
    hp = std::min(hp + cntHp, 100);
    SetSoundVolume(soundGetHp, VOLUME * 2);
    PlaySound(soundGetHp);
}

void Player::updateArmor(const int cntArmor)
{
    armor = std::min(armor + cntArmor, 100);
    SetSoundVolume(soundGetArmor, VOLUME * 2);
    PlaySound(soundGetArmor);
}

void Player::takeDamage(const int damage, const int idOpp, ScoreTable &table)
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
    if (!hp) {
        table.updateKill(idOpp);
        table.updateDeath(id);
        for (auto &idPlayer : whoDmg) {
            if (idPlayer == idOpp) continue;
            table.updateSupport(idPlayer);
        }
        whoDmg.clear();
    }
}

void Player::show(const Vector2 &shift) const
{
    DrawCircle(cameraPos.x + shift.x, cameraPos.y + shift.y, RADIUS, color);
}

void Player::showScope() const
{
    DrawCircle(GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f, RADIUS_SCOPE, darkGray);
    DrawCircle(GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f, RADIUS_SCOPE / 2.0f, RED);
}

void Player::show3DViewInWindow() const
{
    Rectangle wall, crop;
    float frameAngle = 45; // Кадр меняется на каждые 45 градусов
    for (auto iter = drawInfo.rbegin(); iter != drawInfo.rend(); ++iter)
    {
        float curDist, opp_rot, shiftX; int j, curId, stage; const Texture2D *sprite; Vector2 opp_pos;
        std::tie(curDist, opp_rot, opp_pos, shiftX, j, sprite, curId, stage) = *iter;

        float spriteAngle = 0;
        if (curId > 0 && opp_pos.x - cameraPos.x) {
            float angleDistOX = constrainAngle360(RadToDeg(std::atan2(opp_pos.y - cameraPos.y, 
                                                                        opp_pos.x - cameraPos.x)));
            // Арктангенс тангенса угла наклона между прямой Ox и вектором расстояния между игроками = угол между ними
            spriteAngle = opp_rot + 180 - angleDistOX;
        }

        int widthWall = GetRenderWidth() / circlePoints;
        int realHeight = REAL_HEIGHT;
        int heightWall = (int)(DIST_SCREEN / curDist * realHeight);

        wall.width = widthWall;
        wall.height = heightWall;
        wall.x = j * widthWall;
        wall.y = curId ? (GetRenderHeight() / 2.0f - heightWall / 2.5f) : 
                            (GetRenderHeight() - heightWall) / 2.0f;

        crop.x = (curId > 0) ? (int)(TEXTURE_SIZE * std::floor(spriteAngle / frameAngle) + TEXTURE_SIZE * shiftX)
                             : (int)(sprite->width * shiftX);
        crop.y = (curId > 0) ? (stage / DELAY) * TEXTURE_SIZE : 0;
        crop.width = (curId > 0) ? (int)(TEXTURE_SIZE / circlePoints) : (int)(sprite->width / circlePoints);
        crop.height = (curId > 0) ? TEXTURE_SIZE : sprite->height;

        Color tint = changeLightness(lightGray, std::min(0.0f, -curDist));
        DrawTexturePro(*sprite, crop, wall, {0, 0}, 0, tint);
    }
}

void Player::show2DViewInWindow(const Vector2 &shift) const
{
    Color curColor = flashlight; curColor.a = TRANSPARENCY;
    Vector2 pos = {cameraPos.x + shift.x, cameraPos.y + shift.y};
    for (int i = 0; i < circlePoints; ++i) {
        Vector2 curPoint = segment[i];
        curPoint.x += shift.x;
        curPoint.y += shift.y;
        DrawLineEx(pos, curPoint, 1, curColor);
    }
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
    stream << "Current position: (" << cameraPos.x - 2 * THICKNESS_MAP << ", " 
                                    << cameraPos.y - 2 * THICKNESS_MAP << ")";
    DrawTextEx(font, stream.str().c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y + LOG_FONT_SIZE}, 
                                            LOG_FONT_SIZE, 0, tintText);
    stream.str(std::string());
    stream << "Current angle of view: " << constrainAngle360(rotationAngle);
    DrawTextEx(font, stream.str().c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y + 2 * LOG_FONT_SIZE}, 
                                            LOG_FONT_SIZE, 0, tintText);
    stream.str(std::string());
    stream << "Current direction of view: " << int(constrainAngle360(rotationAngle) / 90) + 1 << " quarter";
    DrawTextEx(font, stream.str().c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y + 3 * LOG_FONT_SIZE}, 
                                            LOG_FONT_SIZE, 0, tintText);
    stream.str(std::string());
    stream << "Field of view: " << FOV;
    DrawTextEx(font, stream.str().c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y + 4 * LOG_FONT_SIZE}, 
                                            LOG_FONT_SIZE, 0, tintText);

    std::string strSeenPlayers;
    for (int curId : detectedEnemy)
        strSeenPlayers += " " + std::to_string(curId);

    if (detectedEnemy.empty())
        DrawTextEx(font, "Detected players ID's: None", {LOG_SHIFT_X, LOG_SHIFT_Y + 5 * LOG_FONT_SIZE}, 
                                                        LOG_FONT_SIZE, 0, tintText);
    else
        DrawTextEx(font, ("Detected players ID's:" + strSeenPlayers).c_str(),
                   {LOG_SHIFT_X, LOG_SHIFT_Y + 5 * LOG_FONT_SIZE}, LOG_FONT_SIZE, 0, tintText);

    float distToObj; int objId;
    std::tie(distToObj, std::ignore, std::ignore, std::ignore, 
            std::ignore, std::ignore, objId, std::ignore) = centerObj;
    std::string obj;
    if (objId == 0) obj = "WALL";
    else if (objId > 0) obj = "PLAYER, ID: " + std::to_string(objId);
    else {
        if (objId >= -COUNT_PICKUP_CATEG) obj = "ARMOR";
        else if (objId >= -COUNT_PICKUP_CATEG * 2) obj = "CARTRIDGES";
        else obj = "HEALTH";
    }

    stream.str(std::string());
    stream << "Information about direct object:\n\n\t Seen: " << obj << "\n\n\t Distance: "
           << distToObj << "\n\n\t Object size: " << (objId ? RADIUS * 2 : WALL_SIZE);
    DrawTextEx(font, stream.str().c_str(), {LOG_SHIFT_X, LOG_SHIFT_Y + 6 * LOG_FONT_SIZE}, 
                                            LOG_FONT_SIZE, 0, tintText);
}

void Player::showNickName() const
{
    Vector2 bounds = MeasureTextEx(font, nickName.c_str(), FONT_SIZE_INFO, 0);
    Rectangle bgInfo = {INFO_NICK_X, INFO_NICK_Y, 3 * THICKNESS_FRAME + bounds.x, 
                                                3 * THICKNESS_FRAME + bounds.y};
    DrawRectangleRec(bgInfo, GRAY);
    DrawRectangleLinesEx(bgInfo, THICKNESS_FRAME, BLACK);
    DrawTextEx(font, nickName.c_str(), {INFO_NICK_X + 1.5 * THICKNESS_FRAME, INFO_NICK_Y + 
                2 * THICKNESS_FRAME}, FONT_SIZE_INFO, 0, BLACK);
}

void Player::showHealth() const
{
    DrawRectangleRec(backGroundH, softRed);
    DrawRectangleLinesEx(backGroundH, THICKNESS_FRAME, BLACK);
    DrawTexture(healthTexture, HEALTH_X + THICKNESS_FRAME, HEALTH_Y + THICKNESS_FRAME, WHITE);

    const char *textInfo = TextFormat("%d", hp);
    Vector2 bounds = MeasureTextEx(font, textInfo, FONT_SIZE_INFO, 0);
    Rectangle bgInfo = {INFO_HP_X, INFO_HP_Y, 3 * THICKNESS_FRAME + bounds.x, 3 * THICKNESS_FRAME + bounds.y};
    DrawRectangleRec(bgInfo, GRAY);
    DrawRectangleLinesEx(bgInfo, THICKNESS_FRAME, BLACK);
    DrawTextEx(font, textInfo, {INFO_HP_X + 1.5 * THICKNESS_FRAME, INFO_HP_Y + 2 * THICKNESS_FRAME}, 
                FONT_SIZE_INFO, 0, BLACK);
}

void Player::showArmor() const
{
    DrawRectangleRec(backGroundA, darkBlue);
    DrawRectangleLinesEx(backGroundA, THICKNESS_FRAME, BLACK);
    DrawTexture(armorTexture, ARMOR_X + THICKNESS_FRAME, ARMOR_Y + THICKNESS_FRAME, WHITE);

    const char *textInfo = TextFormat("%d", armor);
    Vector2 bounds = MeasureTextEx(font, textInfo, FONT_SIZE_INFO, 0);
    Rectangle bgInfo = {INFO_ARMOR_X, INFO_ARMOR_Y, 3 * THICKNESS_FRAME + bounds.x, 
                                                    3 * THICKNESS_FRAME + bounds.y};
    DrawRectangleRec(bgInfo, GRAY);
    DrawRectangleLinesEx(bgInfo, THICKNESS_FRAME, BLACK);
    DrawTextEx(font, textInfo, {INFO_ARMOR_X + 1.5 * THICKNESS_FRAME, INFO_ARMOR_Y + 2 * THICKNESS_FRAME}, 
                FONT_SIZE_INFO, 0, BLACK);
}

void Player::setId(const int id) { this->id = id; }

void Player::setRotation(const float angle) { rotationAngle = angle; }

void Player::setPosition(const Vector2 &position)
{
    cameraPos.x = position.x;
    cameraPos.y = position.y;
    mapShiftX = position.x - THICKNESS_MAP * 2;
    mapShiftY = position.y - THICKNESS_MAP * 2;
}

void Player::setFlagMap(bool flag) { map = flag; }

void Player::setFlagShowLog(bool flag) { isLogEnabled = flag; }

void Player::setFlagScoreTable(bool flag) { scoreTable = flag; }

void Player::setLastTimeShoot(const int time) { lastTimeShoot = time; }

void Player::setHealth(const int newHp) { hp = newHp; }

void Player::setArmor(const int newArmor) { armor = newArmor; }

void Player::setTimeDied(const double time) { whenDied = time; }

int Player::getId() const { return id; }

float Player::getRotation() const { return rotationAngle; }

const Vector2& Player::getPosition() const { return cameraPos; }

Vector2 Player::getSize() const { return {2 * RADIUS, 2 * RADIUS}; }

bool Player::getFlagMap() const { return map; }

bool Player::getFlagShowLog() const { return isLogEnabled; }

bool Player::getFlagScoreTable() const { return scoreTable; }

int Player::getLastTimeShoot() const { return lastTimeShoot; }

int Player::getHealth() const { return hp; }

int Player::getArmor() const { return armor; }

double Player::getTimeDied() const { return whenDied; }

const Weapon &Player::getGun() const { return gun; }

Weapon &Player::getGun() { return gun; }

const std::string& Player::getNickName() const { return nickName; }

const Color& Player::getColor() const { return color; }

float Player::getMapShiftX() const { return mapShiftX; }

float Player::getMapShiftY() const { return mapShiftY; }

std::pair<float, int> Player::getInfoCenterObject() const
{
    return std::make_pair(std::get<RAY_INFO::DIST>(centerObj), std::get<RAY_INFO::ID>(centerObj));
}

const std::unordered_set<int> &Player::getDetectedEnemy() const { return detectedEnemy; }

void Player::detectCollision(const std::vector<Rectangle> &objects, Vector2 &delta)
{
    Rectangle copy = {cameraPos.x - RADIUS, cameraPos.y - RADIUS, 2 * RADIUS, 2 * RADIUS};
    copy.x += delta.x;
    copy.y += delta.y;

    std::vector<int> indexCollisions;
    for (size_t i = 0; i < objects.size(); ++i) {
        if (CheckCollisionRecs(copy, objects[i])) indexCollisions.push_back(i);
    }

    if (!indexCollisions.empty()) {
        float delta_x = 0, delta_y = 0;
        for (const auto &ind : indexCollisions) {
            if (delta.x > 0) delta_x += copy.x + copy.width - objects[ind].x;
            else delta_x += objects[ind].x + objects[ind].width - copy.x;

            if (delta.y > 0) delta_y += copy.y + copy.height - objects[ind].y;
            else delta_y += objects[ind].y + objects[ind].height - copy.y;
        }

        if (fabs(delta_x - delta_y) < BLOCK) {
            delta.x = 0; delta.y = 0;
            isMoving = false;
            stageMoving = 0;
        }
        else if (delta_x > delta_y) delta.y = 0;
        else if (delta_y > delta_x) delta.x = 0;
    }
    isMoving = true;
    stageMoving = (stageMoving + 1) % (COUNT_STAGES * DELAY);
    if (!stageMoving) stageMoving++;
    cameraPos.x += delta.x;
    cameraPos.y += delta.y;
    mapShiftX += delta.x;
    mapShiftY += delta.y;
}

Vector2 Player::getCrossPoint(const std::vector<Vector2> &points) const
{
    if (points.empty()) return {1e10, 1e10};
    else if (points.size() == 1) return points.front();
    return (getDist2Points(cameraPos, points.front()) < getDist2Points(cameraPos, points.back())) 
            ? points.front() : points.back();
}

RayInfo Player::getRayDistEnv(const Map &gameMap, const float angle, float &shiftX) const
{
    float pointX = 0, pointY = 0, x0 = 0, y0 = 0;
    ////////////////////////////////////////////////////////// Check HORIZONTAL lines //////////////////////////////////////////////////////////
    float ctgAngle = 1 / tan(angle);
    bool checkH = true;
    RayInfo resultHor = {sightDist, {0, 0}};

    if (sin(angle) > LIMIT) { // Looking DOWN 
        pointY = ((int)((cameraPos.y - gameMap.getFrame().y) / gameMap.getWallSize().y) + 1) *
                     gameMap.getWallSize().y + gameMap.getFrame().y;
        pointX = (pointY - cameraPos.y) * ctgAngle + cameraPos.x;
        y0 = gameMap.getWallSize().y;
        x0 = y0 * ctgAngle;
    }
    else if (sin(angle) < -LIMIT) { // Looking UP
        pointY = (int)((cameraPos.y - gameMap.getFrame().y) / gameMap.getWallSize().y) *
                     gameMap.getWallSize().y + gameMap.getFrame().y - DELTA;
        pointX = (pointY - cameraPos.y) * ctgAngle + cameraPos.x;
        y0 = -gameMap.getWallSize().y;
        x0 = y0 * ctgAngle;
    }
    else {
        pointX = cameraPos.x;
        pointY = cameraPos.y;
        checkH = false;
    }

    Vector2 pointH = {pointX, pointY};
    if (checkH) resultHor = getIntersection(gameMap, pointH, {x0, y0});
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////// Check VERTICAL lines ////////////////////////////////////////////////////////////
    float tanAngle = tan(angle);
    bool checkV = true;
    RayInfo resultVert = {sightDist, {0, 0}};

    if (cos(angle) < -LIMIT) { // Looking LEFT
        pointX = (int)((cameraPos.x - gameMap.getFrame().x) / gameMap.getWallSize().x) *
                     gameMap.getWallSize().x + gameMap.getFrame().x - DELTA;
        pointY = (pointX - cameraPos.x) * tanAngle + cameraPos.y;
        x0 = -gameMap.getWallSize().x;
        y0 = x0 * tanAngle;
    }
    else if (cos(angle) > LIMIT) { // Looking RIGHT
        pointX = ((int)((cameraPos.x - gameMap.getFrame().x) / gameMap.getWallSize().x) + 1) *
                     gameMap.getWallSize().x + gameMap.getFrame().x;
        pointY = (pointX - cameraPos.x) * tanAngle + cameraPos.y;
        x0 = gameMap.getWallSize().x;
        y0 = x0 * tanAngle;
    }
    else {
        pointX = cameraPos.x;
        pointY = cameraPos.y;
        checkV = false;
    }

    Vector2 pointV = {pointX, pointY};
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

float Player::getRayDistObject(const Vector2 &position, const double k, const double b) const
{
    std::vector<Vector2> points;
    const double eps = 1e-2;
    double posPlayerX = position.x;
    double posPlayerY = position.y;

    double aVal = 1.0 + k * k;
    double bVal = -2.0 * posPlayerX + 2.0 * k * b - 2.0 * k * posPlayerY;
    double cVal = posPlayerX * posPlayerX + b * b - 2.0 * posPlayerY * b +
                  posPlayerY * posPlayerY - RADIUS * RADIUS;

    double D = bVal * bVal - 4.0 * aVal * cVal;
    if (std::fabs(D) <= eps) {
        float x = (-bVal) / (2 * aVal);
        float y = k * x + b;
        points.push_back({x, y});
    }
    else if (D > 0) {
        float x1 = (-bVal + std::sqrt(D)) / (2 * aVal);
        float y1 = k * x1 + b;
        float x2 = (-bVal - std::sqrt(D)) / (2 * aVal);
        float y2 = k * x2 + b;
        points.push_back({x1, y1});
        points.push_back({x2, y2});
    }

    Vector2 crossPoint = getCrossPoint(points);
    float distance = getDist2Points(cameraPos, crossPoint);
    float startAngle = constrainAngle360(rotationAngle - FOV / 2);
    float endAngle = constrainAngle360(rotationAngle + FOV / 2);
    float angle = constrainAngle360(RadToDeg(std::atan2(crossPoint.y - cameraPos.y, crossPoint.x - cameraPos.x)));

    bool check = false;
    if (startAngle > endAngle) check = startAngle <= angle || angle <= endAngle;
    else check = startAngle <= angle && angle <= endAngle;

    if (!check || std::fabs(distance - sightDist) <= eps) return sightDist;
    return distance;
}

RayInfo Player::getIntersection(const Map &gameMap, Vector2 &p, const Vector2 &dp) const
{
    int total = gameMap.getMazeSize().x, mapX = 0, mapY = 0;
    std::string notObstacles(".ACH");
    for (int cnt = 0; cnt < total; ++cnt) {
        mapX = (p.y - gameMap.getFrame().y) / gameMap.getWallSize().y;
        mapY = (p.x - gameMap.getFrame().x) / gameMap.getWallSize().x;

        if (mapX >= 0 && mapX < gameMap.getMazeSize().x && mapY >= 0 && mapY < gameMap.getMazeSize().y 
            && notObstacles.find(gameMap.scheme[mapX][mapY]) == std::string::npos) {
            return {getDist2Points(cameraPos, p), {(float)mapX, (float)mapY}};
            break;
        }
        p.x += dp.x;
        p.y += dp.y;
    }
    return {sightDist, {-1, -1}};
}

void Player::calcRayDistEnv(const Map &gameMap)
{
    float curAngle = rotationAngle - FOV / 2;
    float deltaAngle = static_cast<float>(FOV) / circlePoints;

    float sizeWall = gameMap.getWallSize().x;
    for (int i = 0; i < circlePoints; ++i) {
        float shiftX = 0;
        RayInfo cur = getRayDistEnv(gameMap, DegToRad(curAngle), shiftX);
        cur.first *= cos(DegToRad(deltaAngle * i - FOV / 2));

        char type = gameMap.scheme[cur.second.x][cur.second.y];
        drawInfo.push_back(std::make_tuple(cur.first, -1, Vector2{0, 0}, shiftX / sizeWall, i, 
                            gameMap.getTexture(type), 0, 0));
        curAngle += deltaAngle;
    }
}

void Player::calcRayDistPlayers(const std::vector<Player *> &opponents)
{
    float curAngle = rotationAngle - FOV / 2;
    float deltaAngle = static_cast<float>(FOV) / circlePoints;

    for (int i = 0; i < circlePoints; ++i) {
        double k = std::tan(DegToRad(curAngle));
        double b = cameraPos.y - k * cameraPos.x;

        double curAngleViewPlayer = 0, curShiftX = 0;
        float curDist = sightDist; int chosen = -1;
        for (size_t j = 0; j < opponents.size(); ++j) {
            auto [startAngle, angleViewPlayer] = calcAngleFOVObject(RADIUS, opponents[j]->cameraPos);
            float rayDist = getRayDistObject(opponents[j]->getPosition(), k, b);

            if (rayDist < curDist) {
                curShiftX = constrainAngle360(curAngle) > startAngle ? constrainAngle360(curAngle) - startAngle : 360.0 - startAngle + constrainAngle360(curAngle);
                curAngleViewPlayer = angleViewPlayer;
                curDist = rayDist;
                chosen = j;
            }
        }

        if (std::fabs(sightDist - curDist) > 1e-9) {
            curDist *= cos(DegToRad(deltaAngle * i - FOV / 2));
            Vector2 opp_pos = opponents[chosen]->getPosition();
            drawInfo.push_back(std::make_tuple(curDist, opponents[chosen]->getRotation(), opp_pos, 
                                curShiftX / curAngleViewPlayer, i, &opponents[chosen]->texturePlayer,
                                opponents[chosen]->id, opponents[chosen]->stageMoving));
        }
        curAngle += deltaAngle;
    }
}

void Player::calcRayDistPickUps(const std::vector<PickUp> &pickups)
{
    float curAngle = rotationAngle - FOV / 2;
    float deltaAngle = static_cast<float>(FOV) / circlePoints;

    for (int i = 0; i < circlePoints; ++i) {
        double k = std::tan(DegToRad(curAngle));
        double b = cameraPos.y - k * cameraPos.x;

        double curAngleViewPickUp = 0, curShiftX = 0;
        float curDist = sightDist; int chosen = -1;
        for (size_t j = 0; j < pickups.size(); ++j) {

            if (!pickups[j].getFlagActive()) continue;

            auto [startAngle, angleViewPickUp] = calcAngleFOVObject(SIZE_PICKUP, 
                                                pickups[j].getPosition());
            float rayDist = getRayDistObject(pickups[j].getPosition(), k, b);

            if (rayDist < curDist) {
                curShiftX = constrainAngle360(curAngle) > startAngle ? constrainAngle360(curAngle) - 
                            startAngle : 360.0 - startAngle + constrainAngle360(curAngle);
                curAngleViewPickUp = angleViewPickUp;
                curDist = rayDist;
                chosen = j;
            }
        }

        if (std::fabs(sightDist - curDist) > 1e-9) {
            curDist *= cos(DegToRad(deltaAngle * i - FOV / 2));
            drawInfo.push_back(std::make_tuple(curDist, -1, Vector2{0, 0}, curShiftX / curAngleViewPickUp,
                                               i, pickups[chosen].getTexture(), chosen - COUNT_PICKUP_ALL, 0));
        }
        curAngle += deltaAngle;
    }
}

std::pair<float, float> Player::calcAngleFOVObject(const float radius, const Vector2 &position)
{
    double eps = 2e-1;
    double aVal = getDist2Points(cameraPos, position);                       // O1O2
    double bVal = getDist2Points(cameraPos, {cameraPos.x + 1, cameraPos.y}); // O1K
    double cVal = getDist2Points(position, {cameraPos.x + 1, cameraPos.y});  // O2K

    // c ^ 2 = a ^ 2 + b ^ 2 - 2 * a * b * cosC;
    // cosC = (a ^ 2 + b ^ 2 - c ^ 2) / (2 * a * b);
    double centerAngle = 0;
    if (aVal < bVal + cVal && bVal < aVal + cVal && cVal < aVal + bVal) {
        centerAngle = RadToDeg(std::acos((aVal * aVal + bVal * bVal - cVal * cVal) / (2 * aVal * bVal)));
        if (cameraPos.y > position.y) centerAngle = 360.0 - centerAngle;
    }
    else if (cameraPos.x > position.x) centerAngle = 180.0;

    double delta = RadToDeg(std::asin(radius / aVal));
    double startAngle = constrainAngle360(centerAngle - delta - eps);
    double angleFOV = 2 * delta + 2 * eps;

    return {startAngle, angleFOV};
}
