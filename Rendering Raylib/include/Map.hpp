#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include "raylib.h"
#include "Tools.hpp"

#define SIZE_PIXEL_MAP (240)
#define THICKNESS_MAP (10)

using tableChar = std::vector<std::string>;

class Map
{
public:
    tableChar scheme;
    std::vector<std::pair<Rectangle, char>> objects;

    Map(const std::string& filename);
    void findObjects();
    void readTextures(const std::string &filename);
    void showObjectsInWindow() const;
    void showFrame() const;
    const Color& getFillColor() const;
    const Vector2& getMazeSize() const;
    const Vector2& getWallSize() const;
    const Rectangle& getFrame() const;
    const Texture2D* getTexture(char type) const;

private:
    Vector2 mazeSize; Vector2 wallSize;
    Rectangle frame;
    std::unordered_map<char, Texture2D> textures;
    std::unordered_map<char, Color> colors;
};

#endif 