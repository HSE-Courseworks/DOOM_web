#include "ScoreTable.hpp"
//#include "Client.hpp"

extern int MAX_PLAYERS;

ScoreTable::ScoreTable() : gameInfo()
{
    inscriptions = {"Player", "K", "S", "D", "DMG", "KD"};
    //font = LoadFontEx("resources/Calibri.ttf", 30, nullptr, 0);
}

ScoreTable::ScoreTable(const ScoreTable &other) : 
    gameInfo(other.gameInfo), inscriptions(other.inscriptions), font(other.font) {}

ScoreTable &ScoreTable::operator=(const ScoreTable &rht)
{
    gameInfo = rht.gameInfo;
    inscriptions = rht.inscriptions;
    font = rht.font;
    return *this;
}

void ScoreTable::addPlayer(const int id, const std::string &nickName, const Color &color)
{
    gameInfo[id] = std::make_pair(std::make_tuple(nickName, 0, 0, 0, 0, 0), color);
}

void ScoreTable::deletePlayer(const int id)
{
    gameInfo.erase(id);
}

void ScoreTable::updateKill(const int id)
{
    std::get<GAME_INFO::KILL>(gameInfo[id].first)++;

    int kills = std::get<GAME_INFO::KILL>(gameInfo[id].first);
    int deaths = std::get<GAME_INFO::DEATH>(gameInfo[id].first);
    std::get<GAME_INFO::KD>(gameInfo[id].first) = (!deaths) ? kills : static_cast<float>(kills) / deaths;
}

void ScoreTable::updateSupport(const int id)
{
    std::get<GAME_INFO::SUPPORT>(gameInfo[id].first)++;
}

void ScoreTable::updateDeath(const int id)
{
    std::get<GAME_INFO::DEATH>(gameInfo[id].first)++;
    int kills = std::get<GAME_INFO::KILL>(gameInfo[id].first);
    int deaths = std::get<GAME_INFO::DEATH>(gameInfo[id].first);
    std::get<GAME_INFO::KD>(gameInfo[id].first) = static_cast<float>(kills) / deaths;
}

void ScoreTable::updateDamage(const int id, const int damage)
{
    std::get<GAME_INFO::DAMAGE>(gameInfo[id].first) += damage;
}

void ScoreTable::show(const Vector2 &position) const
{
    float x = position.x, y = position.y;
    Rectangle backGroundPlayer = {x + MARGIN, 0, TABLE_SIZE_X - 2 * MARGIN,
                                  (TABLE_SIZE_Y - (MAX_PLAYERS + 1) * MARGIN) / MAX_PLAYERS};
    Rectangle frame = {x, y, TABLE_SIZE_X, TABLE_SIZE_Y};

    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), softGray);
    DrawRectangleRec(frame, colorEarth);
    for (size_t i = 0; i < MAX_PLAYERS; ++i) {
        backGroundPlayer.y = y + MARGIN + i * (MARGIN + backGroundPlayer.height);
        DrawRectangleRec(backGroundPlayer, GRAY);
        DrawRectangleLinesEx(backGroundPlayer, THICKNESS_CEIL * 2, BLACK);
    }

    std::vector<float> parts = {8 * CEIL_SIZE, 2 * CEIL_SIZE,
                                 2 * CEIL_SIZE, 2 * CEIL_SIZE, 
                                 4 * CEIL_SIZE, 3 * CEIL_SIZE};
    std::vector<int> order = getOrderPlayers();
    for (size_t i = 0; i < order.size(); ++i) {
        auto [curGameInfo, color] = gameInfo.at(order[i]);
        float shiftX = x + MARGIN, shiftY = y + MARGIN + i * (MARGIN + backGroundPlayer.height);
        float height = backGroundPlayer.height / 2.0f;
        for (size_t feature = 0; feature < parts.size(); ++feature) {
            Rectangle upperPart = {shiftX, shiftY, parts[feature], height};
            Rectangle downPart = {shiftX, shiftY + height, parts[feature], height};
            DrawRectangleLinesEx(upperPart, THICKNESS_CEIL, BLACK);
            DrawRectangleLinesEx(downPart, THICKNESS_CEIL, BLACK);

            const char *text = nullptr; Color colorNickname = WHITE;
            switch (feature) 
            {
            case GAME_INFO::NICKNAME:
                text = std::get<GAME_INFO::NICKNAME>(curGameInfo).c_str();
                colorNickname = color;
                break;
            case GAME_INFO::KILL:
                text = TextFormat("%d", std::get<GAME_INFO::KILL>(curGameInfo));
                break;
            case GAME_INFO::SUPPORT:
                text = TextFormat("%d", std::get<GAME_INFO::SUPPORT>(curGameInfo));
                break;
            case GAME_INFO::DEATH:
                text = TextFormat("%d", std::get<GAME_INFO::DEATH>(curGameInfo));
                break;
            case GAME_INFO::DAMAGE:
                text = TextFormat("%d", std::get<GAME_INFO::DAMAGE>(curGameInfo));
                break;
            case GAME_INFO::KD:
                text = TextFormat("%.1f", std::get<GAME_INFO::KD>(curGameInfo));
                break;
            }

            Vector2 boundsTitle = MeasureTextEx(font, inscriptions[feature].c_str(), 
                                    FONT_SIZE_INFO, 0);
            Vector2 boundsText = MeasureTextEx(font, text, FONT_SIZE_INFO, 0);

            DrawTextEx(font, inscriptions[feature].c_str(),
                       {upperPart.x + (upperPart.width - boundsTitle.x) / 2.0f,
                        upperPart.y + (upperPart.height - boundsTitle.y) / 2.0f},
                       FONT_SIZE_INFO, 0, BLACK);
            DrawTextEx(font, text, {downPart.x + 
                        (downPart.width - boundsText.x) / 2.0f, downPart.y + 
                        (downPart.height - boundsText.y) / 2.0f}, FONT_SIZE_INFO, 0, colorNickname);

            shiftX += parts[feature];
        }
    }
}

void ScoreTable::reboot() { gameInfo.clear(); }

std::string ScoreTable::getTopPlayer() const
{
    if (gameInfo.size() == 1)
        return std::get<GAME_INFO::NICKNAME>((*gameInfo.begin()).second.first);

    std::vector<int> order = getOrderPlayers();
    std::string nickName;
    const float eps = 1e-6;
    float kd_1, kd_2;
    int kill_1, kill_2, support_1, support_2, dmg_1, dmg_2;
    std::tie(nickName, kill_1, support_1, std::ignore, dmg_1, kd_1) = gameInfo.at(order[0]).first;
    std::tie(std::ignore, kill_2, support_2, std::ignore, dmg_2, kd_2) = gameInfo.at(order[1]).first;

    if (std::fabs(kd_1 - kd_2) < eps && kill_1 == kill_2 && support_1 == support_2 && dmg_1 == dmg_2)
        return " - ";
    return nickName;
}

std::vector<int> ScoreTable::getOrderPlayers() const
{
    std::vector<std::pair<std::tuple<float, int, int, int>, int>> toSort;
    for (auto &[id, info] : gameInfo) {
        toSort.push_back({{std::get<GAME_INFO::KD>(info.first), std::get<GAME_INFO::KILL>(info.first),
        std::get<GAME_INFO::SUPPORT>(info.first), std::get<GAME_INFO::DAMAGE>(info.first)}, id});
    }

    std::sort(toSort.begin(), toSort.end(), [](const auto &p1, const auto &p2) {
        const float eps = 1e-6;
        float kd_1, kd_2;
        int kill_1, kill_2, support_1, support_2, dmg_1, dmg_2;
        std::tie(kd_1, kill_1, support_1, dmg_1) = p1.first;
        std::tie(kd_2, kill_2, support_2, dmg_2) = p2.first;

        return kd_1 - kd_2 > eps || (std::fabs(kd_1 - kd_2) < eps && kill_1 > kill_2) ||
               (std::fabs(kd_1 - kd_2) < eps && kill_1 == kill_2 && support_1 > support_2) ||
               (std::fabs(kd_1 - kd_2) < eps && kill_1 == kill_2 && support_1 == support_2 && dmg_1 > dmg_2); });

    std::vector<int> order;
    for (auto &elem : toSort) order.push_back(elem.second);
    return order;
}
