#pragma once

#include "raylib.h"
#include <map>
#include <memory>
#include <vector>

class Gameplay;

namespace Globals {
    inline Font FONT;
    inline Texture SUN_TEXTURE;
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
    None,
    Sunflower,
    Peashooter,
    Wallnut,
    Cherrybomb,
    Potatomine
};

class Gameplay {
    static const int s_gardenRows = 10;
    static const int s_gardenCols = 5;
    static constexpr float s_gardenPlotWidth = 100;
    static constexpr float s_gardenPlotHeight = (float)Globals::WINDOW_HEIGHT / s_gardenCols - 30;
    static constexpr float s_seedRectSize = 64;
    std::pair<Plant, Rectangle> m_garden[s_gardenRows][s_gardenCols];
    int m_sun = 50;
    const Texture *m_sunTx = &Globals::SUN_TEXTURE;
    float m_totalTime = 0.0f;
    Plant m_seedPack[10] = { Plant::Sunflower, Plant::Peashooter, Plant::Wallnut, Plant::Cherrybomb };
    std::map<Plant, Rectangle> m_seedPackWithRectMap = {};
    float m_timeSinceLastSunSpawn = 0.0f;

    std::vector<Vector2> m_fallingSun = {};
    Plant m_selectedSeed;
public:
    Gameplay();
    ~Gameplay();
    void update(float dt);
    void render();
    void drawPlantRect(const Plant &plant, const Rectangle &rect);
    void spawnFallingSun();
};