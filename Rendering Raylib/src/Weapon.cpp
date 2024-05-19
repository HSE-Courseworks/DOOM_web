#include "Weapon.hpp"

Weapon::Weapon(const std::string& gun, int cartridges, int oneClip, int maxCartridges, int damage) :
    actions(), cartridges(cartridges), oneClip(oneClip), maxCartridges(maxCartridges),
    curCartridge(oneClip), damage(damage)
{
    cartridgesTexture = LoadTexture("resources/cartridges.png");
    redScope = LoadTexture("resources/scope.png");
    font = LoadFontEx("resources/Calibri.ttf", 30, nullptr, 0);
    bgCarts = {CARTRIDGES_X, CARTRIDGES_Y, (float)cartridgesTexture.width + 2 * THICKNESS_FRAME, 
                (float)cartridgesTexture.height + 2 * THICKNESS_FRAME};
    soundShoot = LoadSound("resources/shoot.mp3");
    soundReload = LoadSound("resources/reload.mp3");
    soundEmpty = LoadSound("resources/empty.mp3");

    std::vector<std::string> files = {"/shootRight.png", "/shootLeft.png", "/reloadLeft.png", "/reloadRight.png"};
    for (size_t i = 0; i < files.size(); ++i) {
        actions.push_back(LoadTexture((gun + files[i]).c_str()));
        animFrames.push_back({actions.back().width / WIDTH, actions.back().height / HEIGHT});
    }
}

void Weapon::showGun() const 
{    
    Vector2 pos; Rectangle crop = {0, 0, WIDTH, HEIGHT};
    if (leftHand) pos = {POS_X_LEFT, POS_Y_LEFT};
    else pos = {POS_X_RIGHT, POS_Y_RIGHT};

    int idxR = actions.size() - 1 - leftHand;
    int idxS = leftHand;

    if (!isShooting && !isReloading) {
        DrawTextureRec(actions[idxR], crop, pos, WHITE);
    }
    else if (isReloading) {
        crop.x = (float)WIDTH * (curFrameReload % animFrames[idxR].first);
        crop.y = (float)HEIGHT * (curFrameReload / animFrames[idxR].first);
        DrawTextureRec(actions[idxR], crop, pos, WHITE);
    }
    else if (isShooting) {
        crop.x = (float)WIDTH * (curFrameShoot % animFrames[idxS].first);
        crop.y = (float)HEIGHT * (curFrameShoot / animFrames[idxS].first);
        DrawTextureRec(actions[idxS], crop, pos, WHITE);

        if (hitTarget && curFrameShoot) DrawTexture(redScope, SCOPE_X, SCOPE_Y, WHITE);
    }
}

void Weapon::setFlagLeftHand(bool flag)
{
    leftHand = flag;
}

bool Weapon::getFlagLeftHand() const
{
    return leftHand;
}

void Weapon::showAmmunition() const
{
    DrawRectangleRec(bgCarts, swamp);
    DrawRectangleLinesEx(bgCarts, THICKNESS_FRAME, BLACK);
    DrawTexture(cartridgesTexture, CARTRIDGES_X + THICKNESS_FRAME, CARTRIDGES_Y + THICKNESS_FRAME, WHITE);

    const char *textInfo = TextFormat("%i/%i", curCartridge, cartridges - curCartridge);
	Vector2 bounds = MeasureTextEx(font, textInfo, 30, 0);
    Rectangle bgInfo = {INFO_CART_X, INFO_CART_Y, 3 * THICKNESS_FRAME + bounds.x, 3 * THICKNESS_FRAME + bounds.y};
    DrawRectangleRec(bgInfo, GRAY);
    DrawRectangleLinesEx(bgInfo, THICKNESS_FRAME, BLACK);
    DrawTextEx(font, textInfo, {INFO_CART_X + 1.5 * THICKNESS_FRAME, INFO_CART_Y + 2 * THICKNESS_FRAME}, 30, 0, BLACK);
}

int Weapon::shoot(std::pair<float, int> infoShoot) 
{
    if (isShooting || isReloading) return 0;

    if (curCartridge == 0) {
        SetSoundVolume(soundEmpty, VOLUME);
        PlaySound(soundEmpty);
        return 0;
    }

    int dmg = std::max(0, damage - static_cast<int>(infoShoot.first / SCALE_DAMAGE));
    curCartridge--; cartridges--;
    SetSoundVolume(soundShoot, VOLUME);
    PlaySound(soundShoot);

    isShooting = true;
    if (dmg != 0 && infoShoot.second) hitTarget = true;
    // Запустить анимацию выстрела
    return dmg;
}

void Weapon::reload()
{
    if (isShooting || isReloading || curCartridge == oneClip || cartridges - curCartridge == 0) 
        return;

    SetSoundVolume(soundReload, VOLUME);
    PlaySound(soundReload);

    isReloading = true;
    // Запустить анимацию перезарядки
}

void Weapon::updateNextFrameShoot()
{
    int idx = leftHand;
    if (curFrameShoot >= animFrames[idx].first * animFrames[idx].second)
    {
        curFrameShoot = 0;
        isShooting = false;
        hitTarget = false;
    }
    else curFrameShoot++;
}

void Weapon::updateNextFrameReload()
{
    int idx = actions.size() - 1 - leftHand;
    if (curFrameReload >= animFrames[idx].first * animFrames[idx].second) { 
        curFrameReload = 0;
        curCartridge = std::min(oneClip, cartridges);  
        isReloading = false;
    }
    else curFrameReload++;
}

bool Weapon::checkShooting() const
{
    return isShooting;
}

bool Weapon::checkReloading() const
{
    return isReloading;
}
