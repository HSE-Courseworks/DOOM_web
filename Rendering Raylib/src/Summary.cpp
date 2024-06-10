#include "Summary.hpp"

Summary::Summary()
{
    font = LoadFontEx("resources/Ultraquick.ttf", FONT_SIZE, nullptr, 0);
    title = LoadTexture("resources/gameOver.png");
    alpha = 0;
}

void Summary::show() const
{
    Vector2 position = {(Page::width - TABLE_SIZE_X) / 2.0f, (Page::height - TABLE_SIZE_Y) / 2.0f};
    finalScore.show(position);
    DrawTexture(title, (Page::width - title.width) / 2.0f, INDENT_HEADER * 1.5f, WHITE);

    std::string winner = finalScore.getTopPlayer();
    Vector2 boundsText = MeasureTextEx(font, "Winner: ", FONT_SIZE, SPACING);
    Vector2 boundsNick = MeasureTextEx(font, winner.c_str(), FONT_SIZE * 1.5f, SPACING);

    DrawTextEx(font, "Winner: ", {(Page::width - boundsText.x - boundsNick.x) / 2.0f, 
                position.y + TABLE_SIZE_Y + INDENT_FRAME}, FONT_SIZE, SPACING, WHITE);
    DrawTextEx(font, winner.c_str(), {(Page::width - boundsText.x - boundsNick.x) / 2.0f + 
                boundsText.x, position.y + TABLE_SIZE_Y + INDENT_FRAME - 0.25f * FONT_SIZE}, 
                FONT_SIZE * 1.5f, SPACING, RED);

    if (GetTime() - whenEnd >= TIME_EXIT) {
        Color color = {255, 255, 255, alpha};
        const char *text = "... Press space to menu ...";
        Vector2 boundsText = MeasureTextEx(font, text, FONT_SIZE / 2, SPACING);
        DrawTextEx(font, text, {(Page::width - boundsText.x) / 2.0f, position.y + TABLE_SIZE_Y + 
                    INDENT_FRAME + INDENT_ROW + FONT_SIZE}, FONT_SIZE / 2, SPACING, color);
    }
}

Pages Summary::update()
{
    if (GetTime() - whenEnd >= TIME_EXIT) {
        if (alpha == 0) step = VELOCITY_ALPHA;
        else if (alpha == 255) step = -VELOCITY_ALPHA;
        alpha += step;

        if (IsKeyReleased(KEY_SPACE)) { ShowCursor(); return Pages::MENU; }
    }
    return Pages::SUMMARY;
}

void Summary::setTimeEnd(const double time) { whenEnd = time; }

void Summary::setFinalScoreTable(const ScoreTable &scoreTable) { finalScore = scoreTable; }