#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <raylib.h>

class Data {
public:

private:
    int id, hp = 100, armor = 30, FOV, circlePoints, lastTimeShoot = 0, stageMoving = 0;
    bool map = true, isLogEnabled = false, scoreTable = false, isMoving = false;
    float sightDist, rotationAngle, mapShiftX, mapShiftY;
    double whenDied = 0;
    /* Weapon gun;*/ std::string nickName; /*Color color;*/ Vector2 cameraPos;

    //Texture2D texturePlayer, healthTexture, armorTexture;
};

#endif