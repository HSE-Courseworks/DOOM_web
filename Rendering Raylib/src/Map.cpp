#include "Map.hpp"

Map::Map(const std::string &filename) : scheme(), objects(), pickUps(COUNT_PICKUP_ALL), textures(), colors()
{
    wholeGameMap = LoadTexture("resources/gameMap.png");
    shade = LoadTexture("resources/shade.png");
    frame.width = SIZE_PIXEL_MAP;
    frame.height = SIZE_PIXEL_MAP;
    frame.x = THICKNESS_MAP * 2;
    frame.y = THICKNESS_MAP * 2;

    std::ifstream file(filename, std::ios::in);
    std::string mazeLine;
    while (!file.eof()) {
        file >> mazeLine;
        scheme.push_back(mazeLine);
    }
    file.close();
    mazeSize.x = scheme.front().size(); // Взять длину стенки миникарты
    mazeSize.y = scheme.size();         // Взять ширину стенки миникарты
    wallSize.x = WALL_SIZE;
    wallSize.y = WALL_SIZE;
}

void Map::findObjects()
{
    Rectangle wall = {0, 0, wallSize.x, wallSize.y};
    int N = scheme.size(), M = scheme.front().size();
    
    int posH = 0, posC = COUNT_PICKUP_CATEG, posA = COUNT_PICKUP_CATEG * 2;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (scheme[i][j] == 'H' || scheme[i][j] == 'C' || scheme[i][j] == 'A') {
                float posX = frame.x + j * wallSize.x + WALL_SIZE / 2;
                float posY = frame.y + i * wallSize.y + WALL_SIZE / 2;
                PickUp pickup(scheme[i][j], {posX, posY}, &textures[scheme[i][j]]);

                if (scheme[i][j] == 'H') pickUps[posH++] = pickup;
                else if (scheme[i][j] == 'C') pickUps[posC++] = pickup;
                else pickUps[posA++] = pickup;
            }
            else if (scheme[i][j] != '.') {
                wall.x = frame.x + j * wallSize.x;
                wall.y = frame.y + i * wallSize.y;
                objects.push_back({wall, scheme[i][j]});
            }
        }
    }
}

void Map::readTextures(const std::string &filename)
{
    std::ifstream file(filename, std::ios::in);
    std::string texture, type, colorR, colorG, colorB, colorA;
    while (!file.eof()) {
        file >> type >> texture >> colorR >> colorG >> colorB >> colorA;
        textures[type.front()] = LoadTexture(texture.c_str());
        unsigned char r = std::atoi(colorR.c_str()), g = std::atoi(colorG.c_str());
        unsigned char b = std::atoi(colorB.c_str()), a = std::atoi(colorA.c_str());
        colors[type.front()] = Color{r, g, b, a};
    }
    file.close();
}

void Map::showObjectsInWindow(const float shiftX, const float shiftY) const
{
    Rectangle crop = {shiftX, shiftY, SIZE_PIXEL_MAP, SIZE_PIXEL_MAP};
    DrawTexturePro(wholeGameMap, crop, frame, {0, 0}, 0, WHITE);
    DrawTexture(shade, 2 * THICKNESS_MAP, 2 * THICKNESS_MAP, WHITE);

    // for (auto& object : objects) {
    //     DrawRectangleRec(object.first, colors.at(object.second));
    //     DrawRectangleRoundedLines(object.first, 0, 0, 0.5, BLACK);
    //     DrawRectangleLinesEx(object.first, 0.5, BLACK);
    // }
}

void Map::showFrame() const
{
    DrawRectangleRec(frame, darkGray);
    Rectangle outline;
    outline.width = frame.width + 2 * THICKNESS_MAP;
    outline.height = frame.width + 2 * THICKNESS_MAP;
    outline.x = frame.x - THICKNESS_MAP;
    outline.y = frame.y - THICKNESS_MAP;
    DrawRectangleLinesEx(outline, THICKNESS_MAP, GRAY);
}

const Vector2 &Map::getMazeSize() const { return mazeSize; }

const Vector2 &Map::getWallSize() const { return wallSize; }

const Rectangle &Map::getFrame() const { return frame; }

const Texture2D *Map::getTexture(const char type) const { return &textures.at(type); }

const Texture2D *Map::getMapImage() const { return &wholeGameMap; }
