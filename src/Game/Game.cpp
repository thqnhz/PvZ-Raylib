#include "Game.hpp"

#include <memory>
#include <ranges>
#include <utility>

#include "raylib.h"

namespace Globals {
    void LoadResource() {
        FONT = LoadFontEx(TextFormat("%s/asset/font/creepster.ttf", APP_DIR), 128, 0, 0);
        Image img = LoadImage(TextFormat("%s/asset/misc/sun.png", APP_DIR));
        SUN_TEXTURE = LoadTextureFromImage(img);
    }
    void UnloadResource() {
        UnloadFont(FONT);
        UnloadTexture(SUN_TEXTURE);
    }
}

Game::Game() {
    InitWindow(Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT, "Plants vs. Zombies");
    SetTargetFPS(60);
    Globals::LoadResource();

    m_gameplay = new Gameplay();
}

Game::~Game() {
    Globals::UnloadResource();
    CloseWindow();
}

void Game::run() {
    while (m_isRunning && !WindowShouldClose()) {
        BeginDrawing();
        update();
        EndDrawing();
    }
}

void Game::update() {
    switch (*m_gameState) {
        case GameState::GAMEPLAY: {
            ClearBackground(RAYWHITE);

            // Nothing special, just some calculations to center the text
            const char *text = "Never gonna give you up";
            Vector2 textBoundingBox = MeasureTextEx(Globals::FONT, text, 72, Globals::FONT_SPACING);
            DrawTextEx(Globals::FONT, text, Vector2{Globals::WINDOW_WIDTH / 2.0f - textBoundingBox.x / 2.0f, Globals::WINDOW_HEIGHT / 2.0f - textBoundingBox.y / 2.0f}, 72, Globals::FONT_SPACING, BLACK);

            DrawFPS(100, 10);

            m_gameplay->update();
            break;
        }
        default: {
            m_isRunning = false;
            break;
        }
    }
}

void Game::setGameState(GameState gs) {
    *m_gameState = gs;
}


Gameplay::Gameplay() {
    // Init seed pack
    for (auto [i, plant] : std::views::enumerate(m_seedPack)) {
        Rectangle rect = {
            .x = 0,
            .y = i * s_seedRectSize + 50,
            .width = s_seedRectSize,
            .height = s_seedRectSize
        };
        m_seedPackWithRectMap.insert(std::pair<Plant, Rectangle>(plant, rect));
    }
    // Init garden
    const int spacing = 5;
    const int xOffset = 100;
    const int yOffset = 50;
    for (auto i = 0; i < s_gardenRows; i++) {
        for (auto j = 0; j < s_gardenCols; j++) {
            Rectangle rect = {
                .x = GetScreenWidth() - i*(s_gardenPlotWidth + spacing) - xOffset - s_gardenPlotWidth,
                .y = GetScreenHeight() - j*(s_gardenPlotHeight + spacing) - yOffset - s_gardenPlotHeight,
                .width = s_gardenPlotWidth,
                .height = s_gardenPlotHeight,
            };
            m_garden[i][j] = std::pair<Plant, Rectangle>(Plant::None, rect);
        }
    }
}

Gameplay::~Gameplay() {}

void Gameplay::update() {
    m_totalTime += GetFrameTime();
    // Spawn falling sun
    m_timeSinceLastSunSpawn += GetFrameTime();
    if (m_timeSinceLastSunSpawn >= GetRandomValue(3, 5)) {
        TraceLog(LOG_INFO, "Spawned a sun");
        m_timeSinceLastSunSpawn = 0.0f;
        spawnFallingSun();
    }
    // Update falling suns
    if (!m_fallingSun.empty()) {
        for (auto &fs : m_fallingSun) {
            fs.y += GetFrameTime() * 100;
            if (fs.y >= GetScreenHeight() - m_sunTx->height)
                fs.y = GetScreenHeight() - m_sunTx->height;
        }
    }
    Vector2 mouseLoc = GetMousePosition();
    for (const auto [plant, rect] : m_seedPackWithRectMap) {
        if (CheckCollisionPointRec(mouseLoc, rect)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                m_selectedSeed = plant;
        }
    }
    render();
}

void Gameplay::render() {
    // Draw seed pack
    for (const auto [plant, rect] : m_seedPackWithRectMap) {
        drawPlantRect(plant, rect);
        if (plant == m_selectedSeed && m_selectedSeed != Plant::None)
            DrawRectangleLinesEx(rect, 4, BLACK);
    }
    // Draw Garden
    for (auto i = 0; i < s_gardenRows; i++) {
        for (auto j = 0; j < s_gardenCols; j++) {
            std::pair<Plant, Rectangle> *p = &m_garden[i][j];
            if (CheckCollisionPointRec(GetMousePosition(), p->second)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_selectedSeed != Plant::None) {
                    p->first = m_selectedSeed;
                }
                DrawRectangleRec(p->second, DARKGRAY);
            }
            else
                drawPlantRect(p->first, p->second);
            DrawRectangleLinesEx(p->second, 1.0f, LIGHTGRAY);
        }
    }
    // Draw Sun
    const float sunOffset = (50 - m_sunTx->width) / 2.0f;
    DrawTexture(*m_sunTx, sunOffset, sunOffset, WHITE);
    int sunXPadding = 5;
    DrawTextEx(Globals::FONT, TextFormat("%d", m_sun), { .x = sunOffset + m_sunTx->width + sunXPadding, .y = sunOffset }, m_sunTx->width, Globals::FONT_SPACING, BLACK);
    // Draw falling sun
    for (auto const &fs : m_fallingSun)
        DrawTexture(*m_sunTx, fs.x, fs.y, WHITE);
}

void Gameplay::drawPlantRect(const Plant &plant, const Rectangle &rect) {
    Color color;
    switch (plant) {
    case Plant::Sunflower:
        color = YELLOW;
        break;
    case Plant::Peashooter:
        color = GREEN;
        break;
    case Plant::Cherrybomb:
        color = RED;
        break;
    case Plant::Wallnut:
        color = BROWN;
        break;
    default:
        color = PURPLE;
        break;
    }
    if (!ColorIsEqual(color, PURPLE))
        DrawRectangleRec(rect, color);
}

void Gameplay::spawnFallingSun() {
    int offset = 100;
    Vector2 spawnLocation = {
        .x = (float)GetRandomValue(offset, GetScreenWidth() - offset) ,
        .y = -(float)Globals::SUN_TEXTURE.height
    };
    DrawTexture(*m_sunTx, spawnLocation.x, spawnLocation.y, WHITE);
    m_fallingSun.emplace_back(spawnLocation);
}
