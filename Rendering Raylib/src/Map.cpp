#include "Map.hpp"
#include <iostream>
#include <fstream>

Map::Map(const std::string& filename) : 
    scheme(), objects(), textures(), colors()
{
    wholeGameMap = LoadTexture("resources/gameMap.png");
    shade = LoadTexture("resources/shade.png");
    frame.width = SIZE_PIXEL_MAP; frame.height = SIZE_PIXEL_MAP;
    frame.x = THICKNESS_MAP * 2; frame.y = THICKNESS_MAP * 2;

    std::ifstream file(filename, std::ios::in); std::string mazeLine;
    while (!file.eof()) 
    {
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
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            if (scheme[i][j] != '.')
            {
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
    while (!file.eof()) 
    {
        file >> type >> texture >> colorR >> colorG >> colorB >> colorA;
        textures[type.front()] = LoadTexture(texture.data());
        unsigned char r = std::atoi(colorR.data()), g = std::atoi(colorG.data());
        unsigned char b = std::atoi(colorB.data()), a = std::atoi(colorA.data());
        colors[type.front()] = Color{r, g, b, a};
    }
    file.close();
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

void Map::showObjectsInWindow(float shiftX, float shiftY) const
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

const Vector2& Map::getMazeSize() const
{
    return mazeSize;
}

const Vector2& Map::getWallSize() const
{
    return wallSize;
}

const Rectangle& Map::getFrame() const
{
    return frame;
}

const Texture2D* Map::getTexture(char type) const
{
    return &textures.at(type);
}

const Texture2D* Map::getMapImage() const {
    return &wholeGameMap;
}

//#include <stack>
//#include <random>
//#include <chrono>
//void Map::createMap()
//{
//    using namespace std::chrono;
//
//    std::mt19937 gener(system_clock::now().time_since_epoch().count());
//
//    std::stack<int> stack;
//    std::vector<bool> visited(mazeSize.first * mazeSize.second);
//    stack.push(0); visited[0] = true;
//    while (!stack.empty())
//    {
//        int curVert = stack.top(); stack.pop();
//        std::vector<int> unvisited;
//        for (int i = 0; i < vert[curVert].size(); ++i)
//        {
//            if (!visited[vert[curVert][i]]) 
//                unvisited.push_back(vert[curVert][i]);
//        }
//
//        if (!unvisited.empty())
//        {
//            stack.push(curVert);
//            std::uniform_int_distribution<int> dist(0, unvisited.size() - 1);
//            int chosen = unvisited[dist(gener)];
//            removeElem(vert[curVert], chosen); removeElem(vert[chosen], curVert);
//            visited[chosen] = true;
//            stack.push(chosen);
//        }
//    }
//}
//
//bool find(std::vector<int> arr, int t0)
//{
//    for (int i = 0; i < arr.size(); ++i)
//        if (arr[i] == t0) return true;
//    return false;
//}
//
//void removeElem(std::vector<int>& arr, int t0)
//{
//    for (auto iter = arr.begin(); iter != arr.end();)
//    {
//        if (*iter != t0) iter++;
//        else { iter = arr.erase(iter); break; }
//    }
//}