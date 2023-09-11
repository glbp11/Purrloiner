#include <SDL2/SDL_ttf.h>
#include "AssetManager.h"
#include "Button.h"
#include "Collision.h"
#include "ECS/Components.h"
#include "Game.h"
#include "Map.h"
#include "TextureManager.h"
#include "Vector2D.h"

bool Game::isRunning = false;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::camera;
Manager manager;
AssetManager* Game::assets = new AssetManager(&manager);

Map* map;
TTF_Font* font;
SDL_Color colour = {255, 255, 255};
Button button = Button(208, 266, 96*4, 27*4);
SDL_Rect scoreboardDestRect = {0, 0, 112, 64};
SDL_Texture* scoreboard, *playButton, *restartButton;

int score;
int cameraOffsetX, cameraOffsetY;
int speedTimer, projectileTimer, enemyTimer;
int enemySpeed = 1;

Entity& player(manager.addEntity());

auto& tiles(manager.getGroup(Game::groupMap));
auto& food(manager.getGroup(Game::groupFood));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& chickens(manager.getGroup(Game::groupChickens));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL library successfully initialized" << std::endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            std::cout << "Window successfully created" << std::endl;
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                std::cout << "Renderer successfully created" << std::endl;
                isRunning = true;
                std::cout << "Running" << std::endl;
                if (!TTF_Init()) {
                    std::cout <<"TTF successfully initialized" << std::endl;
                    initAssets();
                }
            }
        }
    }
}

void Game::initAssets() {
    font = TTF_OpenFont("assets/digital-7.regular.ttf", 64);
    assets->AddTexture("terrain", "assets/Grass.png");
    assets->AddTexture("fences", "assets/Fences.png");
    assets->AddTexture("objects", "assets/Objects.png");
    assets->AddTexture("house", "assets/House.png");
    assets->AddTexture("player", "assets/Character.png");
    assets->AddTexture("projectile", "assets/Egg.png");
    assets->AddTexture("enemyChicken", "assets/Chicken.png");
    assets->AddTexture("enemyCow", "assets/Cow.png");
    assets->AddTexture("food", "assets/Milk.png");
    assets->AddTexture("scoreboard", "assets/Scoreboard.png");
    assets->AddTexture("playButton", "assets/Play.png");
    assets->AddTexture("restartButton", "assets/Restart.png");
    assets->AddTexture("titleScreen", "assets/Titlescreen.png");

    scoreboard = assets->GetTexture("scoreboard");
    playButton = assets->GetTexture("playButton");
    restartButton = assets->GetTexture("restartButton");
    
    map = new Map("terrain", Game::MAP_SCALE, Game::TILE_SIZE);
    map->LoadMap("assets/map.map", Game::MAP_SIZE_X, Game::MAP_SIZE_Y);

    Game::camera = {0, 0, Game::WORLD_SIZE_X, Game::WORLD_SIZE_Y};
    cameraOffsetX = Game::WORLD_SIZE_X - Game::CAMERA_WIDTH;
    cameraOffsetY = Game::WORLD_SIZE_Y - Game::CAMERA_HEIGHT;

    player.addComponent<TransformComponent>();
    player.getComponent<TransformComponent>().speed = 3;
    std::vector<std::string> animNames = {"IdleFront", "WalkDown", "IdleBack", "WalkUp", "IdleLeft", "WalkLeft", "IdleRight", "WalkRight"};
    std::vector<int> animFrames = {2, 2, 2, 2, 2, 2, 2, 2};
    player.addComponent<SpriteComponent>("player", animNames, animFrames, 300);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
}

void Game::startGame() {
    score = speedTimer = projectileTimer = enemyTimer = 0;
    enemySpeed = 1;

    player.getComponent<TransformComponent>().velocity.Zero();
    player.getComponent<TransformComponent>().position.x = 15.0f*Game::TILE_SIZE*Game::MAP_SCALE;
    player.getComponent<TransformComponent>().position.y = 12.0f*Game::TILE_SIZE*Game::MAP_SCALE;
    player.getComponent<SpriteComponent>().Play("IdleFront");
    player.getComponent<KeyboardController>().ResetPressing();

    for (auto&e: enemies) {
        e->destroy();
    }
    for (auto&p: projectiles) {
        p->destroy();
    }
    for (auto&f: food) {
        f->destroy();
    }

    manager.refresh();

    assets->SpawnChicken(enemySpeed);
    assets->SpawnCow(enemySpeed);
    assets->SpawnFood("food");
}

void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (button.buttonPressed(event.button) && state != STATE_GAME) {
                state = STATE_GAME;
                startGame();
            }
            break;
        default:
            break;
    }
}

void spawnEnemies() {
    int currentTime = SDL_GetTicks64();
    if (currentTime > enemyTimer) {
        int chance = rand()%10;
        if (chance < 8) {
            Game::assets->SpawnChicken(enemySpeed);
        } else {
            Game::assets->SpawnCow(enemySpeed);
        }
        enemyTimer = currentTime + 10000;
    }
}

void shootEnemyProjectiles() {
    int currentTime = SDL_GetTicks64();
    if (currentTime > projectileTimer) {
        Entity * randomEnemy = chickens[rand()%chickens.size()];
        float projectileDirectionX = player.getComponent<TransformComponent>().position.x - randomEnemy->getComponent<TransformComponent>().position.x;
        float projectileDirectionY = player.getComponent<TransformComponent>().position.y - randomEnemy->getComponent<TransformComponent>().position.y;
        Vector2D projectileDirection = Vector2D(projectileDirectionX, projectileDirectionY).normalize();

        Game::assets->CreateProjectile(randomEnemy->getComponent<TransformComponent>().position, enemySpeed, projectileDirection, "projectile");

        projectileTimer = currentTime + rand()%(10000/(enemySpeed)) + 1;
    }
}

void Game::collisionUpdates() {
    Vector2D playerPosition = player.getComponent<TransformComponent>().position;
    SDL_Rect playerCollider = player.getComponent<ColliderComponent>().collider;

    std::vector<enemyData> enemyUpdateData;
    for (auto&e : enemies) {
        enemyUpdateData.push_back(enemyData());
        enemyUpdateData.back().initialPos = e->getComponent<TransformComponent>().position;
    }

    for (auto&f: food) {
        if (Collision::AABB(playerCollider, f->getComponent<ColliderComponent>().collider)) {
            f->destroy();
            score++;
            assets->SpawnFood("food");
        }
    }

    manager.refresh();
    manager.update();
    
    Vector2D updatedPlayerPosition = player.getComponent<TransformComponent>().position;
    Vector2D playerVector = playerPosition - updatedPlayerPosition;
    playerCollider = player.getComponent<ColliderComponent>().collider;

    int i = 0;
    for (auto&e : enemies) {
        enemyUpdateData[i].updatedPos = e->getComponent<TransformComponent>().position;
        enemyUpdateData[i].vector = enemyUpdateData[i].initialPos - enemyUpdateData[i].updatedPos;
        enemyUpdateData[i].collider = e->getComponent<ColliderComponent>().collider;
        i++;
    }

    for (auto&c : colliders) {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if(Collision::AABB(cCol, playerCollider)) {
            Collision::collisionResolution(&player, playerVector, updatedPlayerPosition, cCol, playerCollider);
        }

        i = 0;
        for (auto&e: enemies) {
            if(Collision::AABB(cCol, enemyUpdateData[i].collider)) {
                Collision::collisionResolution(e, enemyUpdateData[i].vector, enemyUpdateData[i].updatedPos, cCol, enemyUpdateData[i].collider);
                e->getComponent<RandomMovementComponent>().changeDirection();
            }
            i++;
        }

        for (auto&p: projectiles) {
            if (Collision::AABB(cCol, p->getComponent<ColliderComponent>().collider)) {
                p->destroy();
            }
        }
    }

    for (auto&p: projectiles) {
        if (Collision::AABB(playerCollider, p->getComponent<ColliderComponent>().collider)) {
            state = STATE_RESTART;
            p->destroy();
        }
    }

    for (auto&e : enemyUpdateData) {
        if (Collision::AABB(playerCollider, e.collider)) {
            state = STATE_RESTART;
        }
    }
}

void updateEnemySpeed() {
    int currentTime = SDL_GetTicks64();
    if (currentTime > speedTimer) {
        enemySpeed += 1;
        speedTimer = currentTime + 30000;
    }
}

void updateCamera() {
    Game::camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - Game::CAMERA_WIDTH/2 + (Game::DEFAULT_WIDTH*Game::DEFAULT_SCALE)/2);
    Game::camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - Game::CAMERA_HEIGHT/2 + (Game::DEFAULT_HEIGHT*Game::DEFAULT_SCALE)/2);

    if(Game::camera.x < 0)
        Game::camera.x = 0;
    if(Game::camera.y < 0)
        Game::camera.y = 0;
    if(Game::camera.x > cameraOffsetX)
        Game::camera.x = cameraOffsetX;
    if(Game::camera.y > cameraOffsetY)
        Game::camera.y = cameraOffsetY;
}

void Game::update() {
    spawnEnemies();
    shootEnemyProjectiles();
    collisionUpdates();
    updateEnemySpeed();
    updateCamera();
}

void Game::renderMenu() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, assets->GetTexture("titleScreen"), NULL, NULL);
    button.drawButton(playButton);
    SDL_RenderPresent(renderer);
}

void Game::renderRestart() {
    button.drawButton(restartButton);
    SDL_RenderPresent(renderer);
}

void renderScore() {
    SDL_RenderCopy(Game::renderer, scoreboard, nullptr, &scoreboardDestRect);
    SDL_Surface* currentScore = TTF_RenderText_Solid(font, std::to_string(score).c_str(), colour);
    SDL_Texture* message = SDL_CreateTextureFromSurface(Game::renderer, currentScore);
    SDL_Rect messageRect;
    messageRect.x = 15;
    messageRect.y = 5;
    SDL_QueryTexture(message, nullptr, nullptr, &messageRect.w, &messageRect.h);
    SDL_RenderCopy(Game::renderer, message, nullptr, &messageRect);
    SDL_FreeSurface(currentScore);
    SDL_DestroyTexture(message);
}

void Game::render() {
    SDL_RenderClear(renderer);
    for (auto&t : tiles) {
        t->draw();
    }
    for (auto&f : food) {
        f->draw();
    }
    for (auto&p : players) {
        p->draw();
    }
    for (auto&e : enemies) {
        e->draw();
    }
    for (auto&p : projectiles) {
        p->draw();
    }
    renderScore();
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game closed" << std::endl;
}