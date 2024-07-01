#ifndef MAP_HPP
#define MAP_HPP

#include "raylib.h"
#include "Tools.hpp"
#include "PickUp.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <SFML/Network.hpp>

#define SIZE_PIXEL_MAP (240)
#define WALL_SIZE (16)
#define THICKNESS_MAP (10)
#define COUNT_PICKUP_ALL (15)
#define COUNT_PICKUP_CATEG (5)
class Map
{
public:
    std::vector<std::string> scheme;
    std::vector<std::pair<Rectangle, char>> objects;
    std::vector<PickUp> pickUps;

    Map(const std::string &filename);
    void findObjects();
    void readTextures(const std::string &filename);
    void showObjectsInWindow(const float shiftX, const float shiftY) const;
    void showFrame() const;
    
    const Color &getFillColor() const;
    const Vector2 &getMazeSize() const;
    const Vector2 &getWallSize() const;
    const Rectangle &getFrame() const;
    const Texture2D *getTexture(const char type) const;
    const Texture2D *getMapImage() const;

private:
    Vector2 mazeSize, wallSize;
    Rectangle frame;
    Texture2D wholeGameMap, shade;
    std::unordered_map<char, Texture2D> textures;
    std::unordered_map<char, Color> colors;
};



#endif