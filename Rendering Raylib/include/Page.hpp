#ifndef PAGE_HPP
#define PAGE_HPP

#include "raylib.h"

enum Pages { MENU, SETTINGS, GAME, RESURRECTION, PAUSE, SUMMARY, EXIT };

#define INDENT_HEADER (30)
#define THICKNESS_BUTTON (10)
#define INDENT_FRAME (40)
#define INDENT_ROW (20)
#define FONT_SIZE (50)
#define VELOCITY_ALPHA (5)

const Color colorPressed = { 201, 197, 197, 255 };
const Color colorReleased = { 156, 152, 152, 255 };

class Page {
public:
    float width = 1920;
    float height = 1080;
    virtual void show() const = 0;
    virtual Pages update() = 0;
};

#endif