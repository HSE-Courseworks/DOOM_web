#include "Player.hpp"

Player::Player(const Vector2& pos, const float angle, const Color& color, const std::string& texture, const std::string& nickName) : 
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

    cameraPos.x = pos.x;
    cameraPos.y = pos.y; 
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
    DrawCircle(cameraPos.x + shift.x, cameraPos.y + shift.y, RADIUS, color);
}

void Player::detectCollision(const std::vector<Rectangle>& objects, Vector2& delta)
{
    Rectangle copy(cameraPos.x - RADIUS, cameraPos.y - RADIUS, 2 * RADIUS, 2 * RADIUS); 
    copy.x += delta.x; copy.y += delta.y;

    std::vector<int> indexCollisions;
    for (size_t i = 0; i < objects.size(); ++i)
    {
        if (CheckCollisionRecs(copy, objects[i]))
            indexCollisions.push_back(i);
    }
    
    if (!indexCollisions.empty())
    {
        float delta_x = 0, delta_y = 0;
        for (const auto& ind : indexCollisions)
        {
            if (delta.x > 0)
                delta_x += copy.x + copy.width - objects[ind].x;
            else
                delta_x += objects[ind].x + objects[ind].width - copy.x;

            if (delta.y > 0)
                delta_y += copy.y + copy.height - objects[ind].y;
            else
                delta_y += objects[ind].y + objects[ind].height - copy.y;
        }

        if (fabs(delta_x - delta_y) < BLOCK) { 
            delta.x = 0; delta.y = 0; isMoving = false;
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

    if (!notStay) {
        isMoving = false; stageMoving = 0; return;
    }

    std::vector<Rectangle> objects;
    for (size_t i = 0; i < gameMap.objects.size(); ++i) {
        objects.push_back(gameMap.objects[i].first);
    }
    for (auto& [id, player] : players) {
        if (player.id == this->id) continue;
        objects.push_back({player.getPosition().x - RADIUS, player.getPosition().y - RADIUS, 2 * RADIUS, 2 * RADIUS});
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
    //return {object.width, object.height};
    return {2 * RADIUS, 2 * RADIUS};
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

    float distToObj; int objId;
    std::tie(distToObj, std::ignore, std::ignore, std::ignore, 
            std::ignore, std::ignore, objId, std::ignore) = centerObj;
    std::string obj = (objId == 0 ? "WALL" : ("PLAYER, ID: " + std::to_string(objId)));

    stream.str(std::string());
    stream << "Information about direct object:\n\n\t Seen: " << obj << "\n\n\t Distance: " 
           << distToObj << "\n\n\t Object size: " << (objId ? RADIUS * 2 : WALL_SIZE);
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

Vector2 Player::getCrossPoint(const std::vector<Vector2>& points) const 
{  
    if (points.empty()) return {1e10, 1e10};
    else if (points.size() == 1) return points.front();
    return (getDist2Points(cameraPos, points.front()) < getDist2Points(cameraPos, points.back()))
             ? points.front() : points.back();
}

float Player::getRayDistPlayer(const Player* player, const double k, const double b) const
{
    std::vector<Vector2> points; const double eps = 1e-2;
    double posPlayerX = player->getPosition().x;
    double posPlayerY = player->getPosition().y;

    double aVal = 1.0 + k * k;
    double bVal = -2.0 * posPlayerX + 2.0 * k * b - 2.0 * k * posPlayerY;
    double cVal = posPlayerX * posPlayerX + b * b - 2.0 * posPlayerY * b +
                posPlayerY * posPlayerY - RADIUS * RADIUS;

    double D = bVal * bVal - 4.0 * aVal * cVal;
    if (std::fabs(D) <= eps)
    {
        float x = (-bVal) / (2 * aVal);
        float y = k * x + b;
        points.push_back({x, y});
    }
    else if (D > 0)
    {
        float x1 = (-bVal + std::sqrt(D)) / (2 * aVal);
        float y1 = k * x1 + b;
        float x2 = (-bVal - std::sqrt(D)) / (2 * aVal);
        float y2 = k * x2 + b;
        points.push_back({x1, y1}); points.push_back({x2, y2});
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

void Player::show2DViewInWindow(const Vector2& shift) const 
{
    Color curColor = flashlight; curColor.a = TRANSPARENCY;
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
    float frameAngle = 45;  // Кадр меняется на каждые 45 градусов
    for (auto iter = drawInfo.rbegin(); iter != drawInfo.rend(); ++iter)
    {
        float curDist, opp_rot, shiftX; int j, curId, stage; const Texture2D* sprite; Vector2 opp_pos;
        std::tie(curDist, opp_rot, opp_pos, shiftX, j, sprite, curId, stage) = *iter;

        float spriteAngle = 0;
        if (curId && opp_pos.x - cameraPos.x)
        {
            float angleDistOX = constrainAngle360(RadToDeg(std::atan2(opp_pos.y - cameraPos.y, opp_pos.x - cameraPos.x))); 
            // Арктангенс тангенса угла наклона между прямой Ox и вектором расстояния между игроками = угол между ними
            spriteAngle = opp_rot + 180 - angleDistOX;
        }

        int widthWall = GetRenderWidth() / circlePoints;
        int realHeight = REAL_HEIGHT;
        int heightWall = (int)(DIST_SCREEN / curDist * realHeight);

        wall.width = widthWall; wall.height = heightWall;
        wall.x = j * widthWall;
        wall.y = curId ? (GetRenderHeight() / 2.0f - heightWall / 2.5f) : (GetRenderHeight() - heightWall) / 2.0f;

        crop.x = curId ? (int)(TEXTURE_SIZE * std::floor(spriteAngle / frameAngle) + TEXTURE_SIZE * shiftX) 
                       : (int)(sprite->width * shiftX);
        crop.y = curId ? (stage / DELAY) * TEXTURE_SIZE : 0;
        crop.width = curId ? (int)(TEXTURE_SIZE / circlePoints) : (int)(sprite->width / circlePoints);
        crop.height = curId ? TEXTURE_SIZE : sprite->height;

        Color tint = changeLightness(lightGray, std::min(0.0f, -curDist));
        DrawTexturePro(*sprite, crop, wall, {0, 0}, 0, tint);
    }
}

void Player::calcRayDistPlayers(const std::vector<Player*>& opponents)
{
    float curAngle = rotationAngle - FOV / 2;
    float deltaAngle = (float)FOV / circlePoints;
    for (int i = 0; i < circlePoints; ++i)
    {
        double k = std::tan(DegToRad(curAngle));
        double b = cameraPos.y - k * cameraPos.x;
        float curDist = sightDist; double curShiftX = 0; int chosen = -1;

        double curAngleViewPlayer = 0; const double eps = 2e-1;
        for (size_t j = 0; j < opponents.size(); ++j) {

            double aVal = getDist2Points(cameraPos, opponents[j]->getPosition()); // O1O2
            double bVal = getDist2Points(cameraPos, {cameraPos.x + 1, cameraPos.y}); // O1K
            double cVal = getDist2Points(opponents[j]->getPosition(), {cameraPos.x + 1, cameraPos.y}); // O2K

            // c ^ 2 = a ^ 2 + b ^ 2 - 2 * a * b * cosC;
            // cosC = (a ^ 2 + b ^ 2 - c ^ 2) / (2 * a * b);
            double centerAngle = 0;
            if (aVal < bVal + cVal && bVal < aVal + cVal && cVal < aVal + bVal) {
                centerAngle = RadToDeg(std::acos((aVal * aVal + bVal * bVal - cVal * cVal) / (2 * aVal * bVal)));
                if (cameraPos.y > opponents[j]->getPosition().y) centerAngle = 360.0 - centerAngle;
            }
            else if (cameraPos.x > opponents[j]->getPosition().x) centerAngle = 180.0;

            double delta = RadToDeg(std::asin(RADIUS / aVal));
            double startAngle = constrainAngle360(centerAngle - delta - eps);
            double angleViewPlayer = 2 * delta + 2 * eps;

            float rayDist = getRayDistPlayer(opponents[j], k, b);
            if (rayDist < curDist) {
                curShiftX = constrainAngle360(curAngle) > startAngle ? constrainAngle360(curAngle) - startAngle 
                            : 360.0 - startAngle + constrainAngle360(curAngle);
                curAngleViewPlayer = angleViewPlayer;
                curDist = rayDist; chosen = j;
            }
        }

        if (std::fabs(sightDist - curDist) > 1e-9) {
            curDist *= cos(DegToRad(deltaAngle * i - FOV / 2));
            Vector2 opp_pos = opponents[chosen]->getPosition();
            drawInfo.push_back(std::make_tuple(curDist, opponents[chosen]->getRotation(), opp_pos, curShiftX / curAngleViewPlayer,
                                               i, &opponents[chosen]->texturePlayer, opponents[chosen]->id, opponents[chosen]->stageMoving));
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
        drawInfo.push_back(std::make_tuple(cur.first, -1, Vector2(0, 0), shiftX / sizeWall, i, gameMap.getTexture(type), 0, 0));
        curAngle += deltaAngle;
    }
}