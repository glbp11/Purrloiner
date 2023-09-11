#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Vector2D.h"

class AssetManager;
class ColliderComponent;

class Game {
private:
    int const MAP_SIZE_X = 32;
    int const MAP_SIZE_Y = 32;
    int const WORLD_SIZE_X = TILE_SIZE * MAP_SIZE_X * MAP_SCALE;
    int const WORLD_SIZE_Y = TILE_SIZE * MAP_SIZE_Y * MAP_SCALE;

    SDL_Window* window;

    struct enemyData {
        Vector2D vector;
        Vector2D initialPos;
        Vector2D updatedPos;
        SDL_Rect collider;
    };

public:
    static int const DEFAULT_WIDTH = 16;
    static int const DEFAULT_HEIGHT = 16;
    static int const DEFAULT_SCALE = 3;
    static int const TILE_SIZE = 16;
    static int const MAP_SCALE = 3;
    static int const CAMERA_WIDTH = 800;
    static int const CAMERA_HEIGHT = 640;

    static bool isRunning;
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static SDL_Rect camera;
    static AssetManager* assets;
    
    enum groupLabels : std::size_t {
        groupMap,
        groupPlayers,
        groupEnemies,
        groupChickens,
        groupColliders,
        groupProjectiles,
        groupFood
    };

    enum state {
        STATE_MENU,
        STATE_GAME,
        STATE_RESTART,
    } state = STATE_MENU;

    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void startGame();
    void initAssets();
    void handleEvents();
    void collisionUpdates();
    void update();
    void render();
    void renderMenu();
    void renderRestart();
    void clean();

    bool running() { return isRunning; };
};
