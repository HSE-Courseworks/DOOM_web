#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "raylib.h"
#include "Tools.hpp"

#include <string>
#include <vector>

#define MAX_CARTRIDGES (100)
#define POS_GUN_X_LEFT (350)
#define POS_GUN_Y_LEFT (480)
#define POS_GUN_X_RIGHT (770)
#define POS_GUN_Y_RIGHT (480)
#define THICKNESS_FRAME (5)
#define CARTRIDGES_X (10)
#define CARTRIDGES_Y (1000)
#define INFO_CART_X (90)
#define INFO_CART_Y (1015)
#define SCALE_DAMAGE (20)
#define SCOPE_X (935)
#define SCOPE_Y (515)

#define WIDTH_ANIM (800.0f)
#define HEIGHT_ANIM (600.0f)
#define VOLUME (4)

class Weapon
{
public:
    Weapon() = default;
    Weapon(const std::string &gun, const int cartridges, const int oneClip, const int damage);
    
    void showGun() const;
    void showAmmunition() const;
    void updateAmmunition(const int cntCartridges);
    int shoot(const std::pair<float, int>& infoShoot);
    void reload();
    void updateNextFrameShoot();
    void updateNextFrameReload();

    void setFlagLeftHand(bool flag);
    void setAmmunition(const int countCartridges, const int oneClipCount);
    bool getFlagLeftHand() const;
    bool checkShooting() const;
    bool checkReloading() const;

private:
    int cartridges, oneClip, curCartridge, damage;
    bool leftHand = false, isShooting = false, isReloading = false, hitTarget = false;

    int curFrameShoot = 0, curFrameReload = 0;
    std::vector<std::pair<int, int>> animFrames;
    std::vector<Texture2D> actions;

    Rectangle bgCarts;
    Texture2D cartridgesTexture, redScope;
    Sound soundShoot, soundReload, soundEmpty, soundGet;
    Font font;
};

#endif