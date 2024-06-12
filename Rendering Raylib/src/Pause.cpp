#include "Pause.hpp"

Pause::Pause()
{
    float widthButton = BACK_WIDTH - 2 * INDENT_FRAME;
    float heightButton = (BACK_HEIGHT - 2 * INDENT_FRAME - 2 * INDENT_ROW) / 3.0f;

    backGround = {(Page::width - BACK_WIDTH) / 2.0f, (Page::height - BACK_HEIGHT) / 2.0f,
                  BACK_WIDTH, BACK_HEIGHT};
    resumeButton = {(Page::width - widthButton) / 2.0f, backGround.y + INDENT_FRAME,
                    widthButton, heightButton};
    settingsButton = {(Page::width - widthButton) / 2.0f,
                      backGround.y + INDENT_FRAME + INDENT_ROW + heightButton,
                      widthButton, heightButton};
    exitButton = {(Page::width - widthButton) / 2.0f,
                  backGround.y + INDENT_FRAME + 2 * INDENT_ROW + 2 * heightButton,
                  widthButton, heightButton};

    colorResume = colorReleased;
    colorSettings = colorReleased;
    colorExit = colorReleased;
    prevPage = Pages::GAME;
    font = LoadFontEx("resources/Ultraquick.ttf", FONT_SIZE, nullptr, 0);
}

void Pause::show() const
{
    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), softGray);
    DrawRectangleRec(backGround, GRAY);
    DrawRectangleRec(resumeButton, colorResume);
    DrawRectangleLinesEx(resumeButton, THICKNESS_BUTTON, BLACK);
    DrawRectangleRec(settingsButton, colorSettings);
    DrawRectangleLinesEx(settingsButton, THICKNESS_BUTTON, BLACK);
    DrawRectangleRec(exitButton, colorExit);
    DrawRectangleLinesEx(exitButton, THICKNESS_BUTTON, BLACK);

    Vector2 boundsResume = MeasureTextEx(font, "Resume", FONT_SIZE, SPACING);
    Vector2 boundsSettings = MeasureTextEx(font, "Settings", FONT_SIZE, SPACING);
    Vector2 boundsExit = MeasureTextEx(font, "Exit", FONT_SIZE, SPACING);

    Vector2 posPlay = {resumeButton.x + (resumeButton.width - boundsResume.x) / 2.0f,
                       resumeButton.y + (resumeButton.height - boundsResume.y) / 2.0f};
    Vector2 posSettings = {settingsButton.x + (settingsButton.width - boundsSettings.x) / 2.0f,
                           settingsButton.y + (settingsButton.height - boundsSettings.y) / 2.0f};
    Vector2 posExit = {exitButton.x + (exitButton.width - boundsExit.x) / 2.0f,
                       exitButton.y + (exitButton.height - boundsExit.y) / 2.0f};

    DrawTextEx(font, "Resume", posPlay, FONT_SIZE, SPACING, BLACK);
    DrawTextEx(font, "Settings", posSettings, FONT_SIZE, SPACING, BLACK);
    DrawTextEx(font, "Exit", posExit, FONT_SIZE, SPACING, BLACK);
}

Pages Pause::update()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), resumeButton))
            colorResume = colorPressed;
        else if (CheckCollisionPointRec(GetMousePosition(), settingsButton))
            colorSettings = colorPressed;
        else if (CheckCollisionPointRec(GetMousePosition(), exitButton))
            colorExit = colorPressed;
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), resumeButton)) {
            colorResume = colorReleased;
            HideCursor();
            SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
            return prevPage;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), settingsButton)) {
            colorSettings = colorReleased;
            ShowCursor();
            return Pages::SETTINGS;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), exitButton)) {
            colorExit = colorReleased;
            ShowCursor();
            return Pages::MENU;
        }
        else {
            colorResume = colorReleased;
            colorSettings = colorReleased;
            colorExit = colorReleased;
        }
    }
    return Pages::PAUSE;
}

void Pause::setPrevPage(Pages page) { prevPage = page; }