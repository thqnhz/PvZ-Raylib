#pragma once

#include "raylib.h"
#include <map>
#include <memory>

class Gameplay;

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

    Gameplay *m_gameplay;
public:
    Game();
    ~Game();
    void run();
    void update(float dt);
    void setGameState(GameState gs);
};


enum class Plant {
    Sunflower,
    Peashooter,
    Wallnut,
    Cherrybomb,
    Potatomine
};

class Gameplay {
    Plant m_garden[10][5];
    int m_sun = 50;
    float m_totalTime = 0.0f;
    Plant m_seedPack[10] = { Plant::Sunflower, Plant::Peashooter, Plant::Wallnut, Plant::Cherrybomb };
    std::map<Plant, Rectangle> m_seedPackWithRectMap = {};
    Plant *m_selectedSeed;
public:
    Gameplay();
    ~Gameplay();
    void update(float dt);
    void render();
};