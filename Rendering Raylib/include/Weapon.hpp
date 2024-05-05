#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <raylib.h>
#include <string>
#include <vector>
#include "Tools.hpp"

#define POS_X_LEFT (350)
#define POS_Y_LEFT (480)
#define POS_X_RIGHT (770)
#define POS_Y_RIGHT (480)
#define CARTRIDGES_X (10)
#define CARTRIDGES_Y (360)
#define THICKNESS_FRAME (5)
#define INFO_CART_X (90)
#define INFO_CART_Y (375)
#define SCALE_DAMAGE (20)
#define SCOPE_X (935)
#define SCOPE_Y (515)

#define WIDTH (800)
#define HEIGHT (600)
#define VOLUME (4)

class Weapon 
{
public:
    Weapon() = default;
    Weapon(const std::string& gun, int cartridges, int oneClip, int maxCartridges, int damage);
    void showGun() const;
    void setFlagLeftHand(bool flag);
    bool getFlagLeftHand() const;
    void showAmmunition() const;
    int shoot(std::pair<float, int> infoShoot);
    void reload();
    void updateNextFrameShoot();
    void updateNextFrameReload();
    bool checkShooting() const;
    bool checkReloading() const;

private:
    Texture2D cartridgesTexture, redScope;
    Rectangle bgCarts; Font font;
    std::vector<Texture2D> actions;
    Sound soundShoot, soundReload, soundEmpty;
    std::vector<std::pair<int, int>> animFrames;
    int cartridges, oneClip, maxCartridges, curCartridge, damage;
    bool leftHand = true, isShooting = false, isReloading = false;
    int curFrameShoot = 0, curFrameReload = 0;
    bool hitTarget = false;
};

#endif