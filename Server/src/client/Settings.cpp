#include "Settings.hpp"

Settings::Settings()
{
    frame = {(Page::width - FRAME_WIDTH) / 2.0f, (Page::height - FRAME_HEIGTH) / 2.0f + 2 * INDENT_HEADER, FRAME_WIDTH, FRAME_HEIGTH};
    volumeSliderBar = {frame.x + SHIFT_DESC + (SHIFT_DESC - VOLUME_BAR_WIDTH) / 2.0f,
                       frame.y + INDENT_FRAME + 5, VOLUME_BAR_WIDTH, VOLUME_BAR_HEIGHT};
    volume = 0.5f;
    volumeSlider = {volumeSliderBar.x + (volumeSliderBar.width - VOLUME_SLIDER_WIDTH) * volume,
                    volumeSliderBar.y, VOLUME_SLIDER_WIDTH, VOLUME_BAR_HEIGHT};
    colorBack = colorReleased;
    //font = LoadFontEx("resources/Ultraquick.ttf", FONT_SIZE, nullptr, 0);
    //back = LoadTexture("resources/back.png");
    //title = LoadTexture("resources/titleSettings.png");
}

void Settings::show() const
{
    DrawCircle(BACK_BUTTON_X, BACK_BUTTON_Y, BACK_BUTTON_RADIUS + THICKNESS_BUTTON / 2.0f, BLACK);
    DrawCircle(BACK_BUTTON_X, BACK_BUTTON_Y, BACK_BUTTON_RADIUS, colorBack);
    DrawTexture(back, BACK_BUTTON_X - back.width / 2.0f,
                BACK_BUTTON_Y - back.height / 2.0f, WHITE);

    Vector2 posTitle = {(Page::width - title.width) / 2.0f, INDENT_HEADER};
    DrawTexture(title, posTitle.x, posTitle.y, WHITE);
    DrawRectangleRec(frame, colorReleased);
    DrawRectangleLinesEx(frame, THICKNESS_BUTTON, BLACK);

    std::vector<std::string> text = {"> Volume", "> M", "> TAB", "> Q", "> E", "> L", "> R", "> LMB", "> MMB"};
    for (size_t i = 0; i < text.size(); ++i) {
        Vector2 pos = {frame.x + INDENT_FRAME,
                       frame.y + INDENT_FRAME + INDENT_ROW * i + FONT_SIZE * i};
        DrawTextEx(font, text[i].c_str(), pos, FONT_SIZE, SPACING, BLACK);
    }

    DrawRectangleRec(volumeSliderBar, GRAY);
    DrawRectangleRec(volumeSlider, colorPressed);

    std::vector<std::string> description = {"Show whole map", "Show score table",
                                            "Result of (M + TAB)", "Get pickup", "Show logs",
                                            "Reload", "Shoot", "Swap hands"};
    std::vector<Vector2> bounds(description.size());
    std::transform(description.begin(), description.end(), bounds.begin(), [&](const std::string &str)
                   { return MeasureTextEx(font, str.c_str(), FONT_SIZE / 1.5, SPACING); });
    for (size_t i = 0; i < description.size(); ++i) {
        Vector2 pos = {frame.x + SHIFT_DESC + (SHIFT_DESC - bounds[i].x) / 2.0f,
                       frame.y + INDENT_FRAME + INDENT_ROW * (i + 1) + FONT_SIZE * (i + 1) +
                           (FONT_SIZE - bounds[i].y) / 2.0f};
        DrawTextEx(font, description[i].c_str(), pos, FONT_SIZE / 1.5, SPACING, BLACK);
    }
}

Pages Settings::update()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointCircle(GetMousePosition(), 
            {BACK_BUTTON_X, BACK_BUTTON_Y}, BACK_BUTTON_RADIUS))
            colorBack = colorPressed;
        if (CheckCollisionPointRec(GetMousePosition(), volumeSliderBar)) {
            volumeSlider.x = GetMouseX() - volumeSlider.width / 2;
            if (volumeSlider.x < volumeSliderBar.x)
                volumeSlider.x = volumeSliderBar.x;
            if (volumeSlider.x > volumeSliderBar.x + volumeSliderBar.width - volumeSlider.width)
                volumeSlider.x = volumeSliderBar.x + volumeSliderBar.width - volumeSlider.width;

            volume = (volumeSlider.x - volumeSliderBar.x) / (volumeSliderBar.width - volumeSlider.width);
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointCircle(GetMousePosition(), 
            {BACK_BUTTON_X, BACK_BUTTON_Y}, BACK_BUTTON_RADIUS)) {
            ShowCursor();
            colorBack = colorReleased;
            return prevPage;
        }
        else colorBack = colorReleased;
    }

    return Pages::SETTINGS;
}

void Settings::setPrevPage(Pages page) { prevPage = page; }

float Settings::getVolume() { return volume; }
