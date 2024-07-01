#include "Resurrection.hpp"

Resurrection::Resurrection()
{
    //backGround = LoadTexture("resources/bgDeath.png");
    //title = LoadTexture("resources/titleResurrection.png");
    //font = LoadFontEx("resources/Ultraquick.ttf", FONT_SIZE, nullptr, 0);
    alpha = 0;
}

void Resurrection::show() const
{
    DrawTexture(backGround, 0, 0, WHITE);
    DrawTexture(title, (Page::width - title.width) / 2.0f, INDENT_HEADER * 1.5, WHITE);

    if (GetTime() - whenDied < TIME_RESURRECTION) {
        Vector2 bounds = MeasureTextEx(font, "Restart after ...", FONT_SIZE, SPACING);
        DrawTextEx(font, "Restart after ...", {(Page::width - bounds.x) / 2.0f, 
                    (Page::height - bounds.y) / 2.0f}, FONT_SIZE, SPACING, WHITE);

        int leftSeconds = static_cast<int>(TIME_RESURRECTION - (GetTime() - whenDied));
        const char *text = TextFormat("%d", leftSeconds);
        Vector2 boundsText = MeasureTextEx(font, text, FONT_SIZE * 2, SPACING);
        DrawTextEx(font, text, {(Page::width - boundsText.x) / 2.0f, 
                    (Page::height - boundsText.y) / 2.0f + FONT_SIZE + INDENT_HEADER}, 
                    FONT_SIZE * 2, SPACING, WHITE);
    }
    else {
        Color color = {255, 255, 255, alpha};
        const char *text = "... Press space to restart ...";
        Vector2 boundsText = MeasureTextEx(font, text, FONT_SIZE, SPACING);
        DrawTextEx(font, text, {(Page::width - boundsText.x) / 2.0f, 
                    (Page::height - boundsText.y) / 2.0f}, FONT_SIZE, SPACING, color);
    }
}

Pages Resurrection::update()
{
    if (IsKeyReleased(KEY_ESCAPE)) { ShowCursor(); return Pages::PAUSE; }

    if (GetTime() - whenDied >= TIME_RESURRECTION) {
        if (alpha == 0) step = VELOCITY_ALPHA;
        else if (alpha == 255) step = -VELOCITY_ALPHA;
        alpha += step;

        if (IsKeyReleased(KEY_SPACE)) { HideCursor(); return Pages::GAME; }
    }
    return Pages::RESURRECTION;
}

void Resurrection::setTimeDied(const double time) { whenDied = time; }