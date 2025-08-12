#include "Game.hpp"

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