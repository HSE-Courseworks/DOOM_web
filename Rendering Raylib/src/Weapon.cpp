#include "Weapon.hpp"

Weapon::Weapon(const std::string &gun, const int cartridges, const int oneClip, const int damage) : 
    cartridges(cartridges), oneClip(oneClip), curCartridge(oneClip), damage(damage)
{
    cartridgesTexture = LoadTexture("resources/cartridges.png");
    redScope = LoadTexture("resources/scope.png");
    font = LoadFontEx("resources/Calibri.ttf", FONT_SIZE_INFO, nullptr, 0);
    bgCarts = {CARTRIDGES_X, CARTRIDGES_Y, cartridgesTexture.width + 2.0f * THICKNESS_FRAME,
                cartridgesTexture.height + 2.0f * THICKNESS_FRAME};
    soundShoot = LoadSound("resources/shoot.mp3");
    soundReload = LoadSound("resources/reload.mp3");
    soundEmpty = LoadSound("resources/empty.mp3");
    soundGet = LoadSound("resources/getCartridges.mp3");

    std::vector<std::string> files = {"/shootRight.png", "/shootLeft.png", 
                                        "/reloadLeft.png", "/reloadRight.png"};
    for (size_t i = 0; i < files.size(); ++i) {
        actions.push_back(LoadTexture((gun + files[i]).c_str()));
        animFrames.push_back({actions.back().width / WIDTH_ANIM, actions.back().height / HEIGHT_ANIM});
    }
}

void Weapon::showGun() const
{
    Vector2 pos = leftHand ? Vector2{POS_GUN_X_LEFT, POS_GUN_Y_LEFT} : 
                            Vector2{POS_GUN_X_RIGHT, POS_GUN_Y_RIGHT};
    Rectangle crop = {0, 0, WIDTH_ANIM, HEIGHT_ANIM};
    int idxR = actions.size() - 1 - leftHand;
    int idxS = leftHand;

    if (!isShooting && !isReloading) {
        DrawTextureRec(actions[idxR], crop, pos, WHITE);
    }
    else if (isReloading) {
        crop.x = WIDTH_ANIM * (curFrameReload % animFrames[idxR].first);
        crop.y = HEIGHT_ANIM * (curFrameReload / animFrames[idxR].first);
        DrawTextureRec(actions[idxR], crop, pos, WHITE);
    }
    else if (isShooting) {
        crop.x = WIDTH_ANIM * (curFrameShoot % animFrames[idxS].first);
        crop.y = HEIGHT_ANIM * (curFrameShoot / animFrames[idxS].first);
        DrawTextureRec(actions[idxS], crop, pos, WHITE);

        if (hitTarget && curFrameShoot) DrawTexture(redScope, SCOPE_X, SCOPE_Y, WHITE);
    }
}

void Weapon::showAmmunition() const
{
    DrawRectangleRec(bgCarts, swamp);
    DrawRectangleLinesEx(bgCarts, THICKNESS_FRAME, BLACK);
    DrawTexture(cartridgesTexture, CARTRIDGES_X + THICKNESS_FRAME, 
                                    CARTRIDGES_Y + THICKNESS_FRAME, WHITE);

    const char *textInfo = TextFormat("%d/%d", curCartridge, cartridges);
    Vector2 bounds = MeasureTextEx(font, textInfo, FONT_SIZE_INFO, 0);
    Rectangle bgInfo = {INFO_CART_X, INFO_CART_Y, 3 * THICKNESS_FRAME + bounds.x, 
                                                    3 * THICKNESS_FRAME + bounds.y};
    DrawRectangleRec(bgInfo, GRAY);
    DrawRectangleLinesEx(bgInfo, THICKNESS_FRAME, BLACK);
    DrawTextEx(font, textInfo, {INFO_CART_X + 1.5f * THICKNESS_FRAME, 
                INFO_CART_Y + 2 * THICKNESS_FRAME}, FONT_SIZE_INFO, 0, BLACK);
}

void Weapon::updateAmmunition(const int cntCartridges)
{
    cartridges = std::min(cartridges + cntCartridges, MAX_CARTRIDGES);
    SetSoundVolume(soundGet, VOLUME * 2);
    PlaySound(soundGet);
}

int Weapon::shoot(const std::pair<float, int>& infoShoot)
{
    if (isShooting || isReloading) return 0;

    if (curCartridge == 0) {
        SetSoundVolume(soundEmpty, VOLUME);
        PlaySound(soundEmpty);
        return 0;
    }

    int dmg = std::max(0, damage - static_cast<int>(infoShoot.first / SCALE_DAMAGE));
    curCartridge--;
    SetSoundVolume(soundShoot, VOLUME);
    PlaySound(soundShoot);

    isShooting = true;
    if (dmg != 0 && infoShoot.second > 0) { hitTarget = true; }
    // Запускается анимацию выстрела
    return dmg;
}

void Weapon::reload()
{
    if (isShooting || isReloading || curCartridge == oneClip || cartridges == 0)
        return;

    SetSoundVolume(soundReload, VOLUME);
    PlaySound(soundReload);

    isReloading = true;
    // Запускается анимацию перезарядки
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
    if (curFrameReload >= animFrames[idx].first * animFrames[idx].second)
    {
        curFrameReload = 0;
        cartridges += curCartridge;
        curCartridge = std::min(oneClip, cartridges);
        cartridges -= curCartridge;
        isReloading = false;
    }
    else curFrameReload++;
}

void Weapon::setFlagLeftHand(bool flag) { leftHand = flag; }

void Weapon::setAmmunition(const int countCartridges, const int oneClipCount)
{
    cartridges = countCartridges;
    oneClip = oneClipCount;
    curCartridge = oneClipCount;
}

bool Weapon::getFlagLeftHand() const { return leftHand; }

bool Weapon::checkShooting() const { return isShooting; }

bool Weapon::checkReloading() const { return isReloading; }