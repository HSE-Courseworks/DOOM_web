#include "Menu.hpp"

Menu::Menu()
{
    startButton = {(Page::width - BUTTON_WIDTH_MENU) / 2.0f, (Page::height / 2.0f),
                   BUTTON_WIDTH_MENU, BUTTON_HEIGHT_MENU};
    settingsButton = {(Page::width - BUTTON_WIDTH_MENU) / 2.0f,
                      (Page::height / 2.0f) + INDENT_ROW + BUTTON_HEIGHT_MENU,
                      BUTTON_WIDTH_MENU, BUTTON_HEIGHT_MENU};
    exitButton = {(Page::width - BUTTON_WIDTH_MENU) / 2.0f,
                  (Page::height / 2.0f) + 2 * INDENT_ROW + 2 * BUTTON_HEIGHT_MENU,
                  BUTTON_WIDTH_MENU, BUTTON_HEIGHT_MENU};
    colorStart = colorReleased;
    colorSettings = colorReleased;
    colorExit = colorReleased;
    //font = LoadFontEx("resources/Ultraquick.ttf", FONT_SIZE, nullptr, 0);
    //title = LoadTexture("resources/titleMenu.png");
}

void Menu::show() const
{
    Vector2 posTitle = {(Page::width - title.width) / 2.0f, INDENT_HEADER};
    DrawTexture(title, posTitle.x, posTitle.y, WHITE);
    DrawRectangleRec(startButton, colorStart);
    DrawRectangleLinesEx(startButton, THICKNESS_BUTTON, BLACK);
    DrawRectangleRec(settingsButton, colorSettings);
    DrawRectangleLinesEx(settingsButton, THICKNESS_BUTTON, BLACK);
    DrawRectangleRec(exitButton, colorExit);
    DrawRectangleLinesEx(exitButton, THICKNESS_BUTTON, BLACK);

    Vector2 boundsPlay = MeasureTextEx(font, "Play", FONT_SIZE, SPACING);
    Vector2 boundsSettings = MeasureTextEx(font, "Settings", FONT_SIZE, SPACING);
    Vector2 boundsExit = MeasureTextEx(font, "Exit", FONT_SIZE, SPACING);

    Vector2 posPlay = {startButton.x + (startButton.width - boundsPlay.x) / 2.0f,
                       startButton.y + (startButton.height - boundsPlay.y) / 2.0f};
    Vector2 posSettings = {settingsButton.x + (settingsButton.width - boundsSettings.x) / 2.0f,
                           settingsButton.y + (settingsButton.height - boundsSettings.y) / 2.0f};
    Vector2 posExit = {exitButton.x + (exitButton.width - boundsExit.x) / 2.0f,
                       exitButton.y + (exitButton.height - boundsExit.y) / 2.0f};

    DrawTextEx(font, "Play", posPlay, FONT_SIZE, SPACING, BLACK);
    DrawTextEx(font, "Settings", posSettings, FONT_SIZE, SPACING, BLACK);
    DrawTextEx(font, "Exit", posExit, FONT_SIZE, SPACING, BLACK);
}

Pages Menu::update()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), startButton)) 
            colorStart = colorPressed;
        else if (CheckCollisionPointRec(GetMousePosition(), settingsButton))
            colorSettings = colorPressed;
        else if (CheckCollisionPointRec(GetMousePosition(), exitButton))
            colorExit = colorPressed;
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), startButton)) {
            colorStart = colorReleased;
            HideCursor();
            SetMousePosition(GetRenderWidth() / 2, GetRenderHeight() / 2);
            return Pages::GAME;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), settingsButton)) {
            colorSettings = colorReleased;
            ShowCursor();
            return Pages::SETTINGS;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), exitButton)) {
            colorSettings = colorReleased;
            ShowCursor();
            return Pages::EXIT;
        }
        else {
            colorStart = colorReleased;
            colorSettings = colorReleased;
            colorExit = colorReleased;
        }
    }
    return Pages::MENU;
}
