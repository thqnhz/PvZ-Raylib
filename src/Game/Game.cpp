#include "Game.hpp"

#include <ranges>
#include <utility>

#include "raylib.h"

namespace Globals {
    void LoadResource() {
        FONT = LoadFontEx(TextFormat("%s/asset/font/creepster.ttf", GetWorkingDirectory()), 128, 0, 0);
    }
    void UnloadResource() {
        UnloadFont(FONT);
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
        float dt = GetFrameTime();
        update(dt);
        EndDrawing();
    }
}

void Game::update(float dt) {
    switch (*m_gameState) {
        case GameState::GAMEPLAY: {
            ClearBackground(RAYWHITE);

            // Nothing special, just some calculations to center the text
            const char *text = "Never gonna give you up";
            Vector2 textBoundingBox = MeasureTextEx(Globals::FONT, text, 72, Globals::FONT_SPACING);
            DrawTextEx(Globals::FONT, text, Vector2{Globals::WINDOW_WIDTH / 2.0f - textBoundingBox.x / 2.0f, Globals::WINDOW_HEIGHT / 2.0f - textBoundingBox.y / 2.0f}, 72, Globals::FONT_SPACING, BLACK);

            DrawFPS(10, 10);

            m_gameplay->update(dt);
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
    // Draw seed pack
    for (auto [i, plant] : std::views::enumerate(m_seedPack)) {
        Rectangle rect = {
            .x = 0,
            .y = i * s_seedRectSize + 50,
            .width = s_seedRectSize,
            .height = s_seedRectSize
        };
        m_seedPackWithRectMap.insert(std::pair<Plant, Rectangle>(plant, rect));
    }
    // Draw garden
    for (auto i = 0; i < s_gardenRows; i++) {
        for (auto j = 0; j < s_gardenCols; j++) {
            Rectangle rect = {
                .x = GetScreenWidth() - i*(s_gardenPlotWidth + 5) - 100,
                .y = GetScreenHeight() - j*(s_gardenPlotHeight + 5) - 50,
                .width = -s_gardenPlotWidth,
                .height = -s_gardenPlotHeight,
            };
            m_garden[i][j] = std::pair<Plant, Rectangle>(Plant::None, rect);
        }
    }
}

Gameplay::~Gameplay() {}

void Gameplay::update(float dt) {
    m_totalTime += dt;
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
    for (const auto [plant, rect] : m_seedPackWithRectMap) {
        switch (plant) {
        case Plant::Sunflower:
            DrawRectangleRec(rect, YELLOW);
            break;
        case Plant::Peashooter:
            DrawRectangleRec(rect, GREEN);
            break;
        case Plant::Cherrybomb:
            DrawRectangleRec(rect, RED);
            break;
        case Plant::Wallnut:
            DrawRectangleRec(rect, BROWN);
            break;
        default:
            DrawRectangleRec(rect, PURPLE);
            break;
        }
        if (plant == m_selectedSeed)
            DrawRectangleLinesEx(rect, 4, BLACK);
    }
    for (auto i = 0; i < s_gardenRows; i++) {
        for (auto j = 0; j < s_gardenCols; j++) {
            std::pair<Plant, Rectangle> p = m_garden[i][j];
            DrawRectangleLinesEx(p.second, 0.1f, {0, 82, 172, 50});
        }
    }
}