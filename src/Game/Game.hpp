#pragma once

#include "raylib.h"
#include <memory>

namespace Globals {
    inline Font FONT;
    inline static const int WINDOW_WIDTH = 1280;
    inline static const int WINDOW_HEIGHT = 720;
    inline static const int FONT_SPACING = 1;
    inline const char *APP_DIR = GetWorkingDirectory();

    void LoadResource();
    void UnloadResource();
};

enum class GameState {
    LOADING,
    MAIN_MENU,
    GAMEPLAY,
    EXITING
};

class Game {
    std::unique_ptr<GameState> m_gameState = std::make_unique<GameState>(GameState::GAMEPLAY);
    bool m_isRunning = true;
public:
    Game();
    ~Game();
    void run();
    void update(float dt);
    void setGameState(GameState gs);
};