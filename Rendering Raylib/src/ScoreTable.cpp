#include "ScoreTable.hpp"

ScoreTable::ScoreTable() : gameInfo() {
    inscriptions = {"Player", "K", "S", "D", "DMG", "KD"};
    frame = {SCORE_X, SCORE_Y, TABLE_SIZE_X, TABLE_SIZE_Y};
    font = LoadFontEx("resources/Calibri.ttf", 30, nullptr, 0);
}

void ScoreTable::addPlayer(const int id, const std::string& nickName, const Color& color) {
    gameInfo[id] = std::make_pair(std::make_tuple(nickName, 0, 0, 0, 0, 0), color);
}

void ScoreTable::deletePlayer(const int id) {
    gameInfo.erase(id);
}

void ScoreTable::updateKill(const int id) {
    std::get<GAME_INFO::KILL>(gameInfo[id].first)++;

    int kills = std::get<GAME_INFO::KILL>(gameInfo[id].first);
    int deaths = std::get<GAME_INFO::DEATH>(gameInfo[id].first);
    std::get<GAME_INFO::KD>(gameInfo[id].first) = (!deaths) ? kills : (float)kills / deaths;
}

void ScoreTable::updateSupport(const int id) {
    std::get<GAME_INFO::SUPPORT>(gameInfo[id].first)++;
}

void ScoreTable::updateDeath(const int id) {
    std::get<GAME_INFO::DEATH>(gameInfo[id].first)++;
    int kills = std::get<GAME_INFO::KILL>(gameInfo[id].first);
    int deaths = std::get<GAME_INFO::DEATH>(gameInfo[id].first);
    std::get<GAME_INFO::KD>(gameInfo[id].first) = (float)kills / deaths;
}

void ScoreTable::updateDamage(const int id, const int damage) {
    std::get<GAME_INFO::DAMAGE>(gameInfo[id].first) += damage;
}

void ScoreTable::show() const {
    Rectangle backGroundPlayer = {SCORE_X + MARGIN, 0, TABLE_SIZE_X - 2 * MARGIN, 
                    (TABLE_SIZE_Y - (MAX_PLAYERS + 1) * MARGIN) / MAX_PLAYERS};
    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), softGray);
    DrawRectangleRec(frame, colorEarth);
    for (size_t i = 0; i < MAX_PLAYERS; ++i) {
        backGroundPlayer.y = SCORE_Y + MARGIN + i * (MARGIN + backGroundPlayer.height);
        DrawRectangleRec(backGroundPlayer, GRAY);
        DrawRectangleLinesEx(backGroundPlayer, THICKNESS_CEIL + 1, BLACK);
    }

    std::vector<float> parts = {160, 40, 40, 40, 80, 60};
    std::vector <std::pair<float, int>> order;
    for (auto& [id, info] : gameInfo) order.push_back({std::get<GAME_INFO::KD>(info.first), id});
    std::sort(order.begin(), order.end(), [](const auto& p1, const auto& p2){return p1.first > p2.first;});

    for (size_t i = 0; i < order.size(); ++i)
    {
        auto [curGameInfo, color] = gameInfo.at(order[i].second);
        float shiftX = SCORE_X + MARGIN, shiftY = SCORE_Y + MARGIN + i * (MARGIN + backGroundPlayer.height);
        float height = backGroundPlayer.height / 2.0f;
        for (size_t feature = 0; feature < parts.size(); ++feature) {
            Rectangle upperPart = {shiftX, shiftY, parts[feature], height};
            Rectangle downPart = {shiftX, shiftY + height, parts[feature], height};
            DrawRectangleLinesEx(upperPart, THICKNESS_CEIL, BLACK);
            DrawRectangleLinesEx(downPart, THICKNESS_CEIL, BLACK);

            const char* text = nullptr; Color textColor = WHITE;
            switch (feature)
            {
            case GAME_INFO::NICKNAME:
                text = std::get<GAME_INFO::NICKNAME>(curGameInfo).c_str(); textColor = color; break;
            case GAME_INFO::KILL:
                text = TextFormat("%d", std::get<GAME_INFO::KILL>(curGameInfo)); break;
            case GAME_INFO::SUPPORT:
                text = TextFormat("%d", std::get<GAME_INFO::SUPPORT>(curGameInfo)); break;
            case GAME_INFO::DEATH:
                text = TextFormat("%d", std::get<GAME_INFO::DEATH>(curGameInfo)); break;
            case GAME_INFO::DAMAGE:
                text = TextFormat("%d", std::get<GAME_INFO::DAMAGE>(curGameInfo)); break;
            case GAME_INFO::KD:
                text = TextFormat("%.1f", std::get<GAME_INFO::KD>(curGameInfo)); break;  
            }

            Vector2 boundsTitle = MeasureTextEx(font, inscriptions[feature].c_str(), 30, 0);           
            Vector2 boundsText = MeasureTextEx(font, text, 30, 0);

            DrawTextEx(font, inscriptions[feature].c_str(), {upperPart.x + (upperPart.width - boundsTitle.x) / 2.0f, 
                       upperPart.y + (upperPart.height - boundsTitle.y) / 2.0f}, 30, 0, BLACK);

            DrawTextEx(font, text, {downPart.x + (downPart.width - boundsText.x) / 2.0f, 
                       downPart.y + (downPart.height - boundsText.y) / 2.0f}, 30, 0, textColor);

            shiftX += parts[feature];
        }
    }
}