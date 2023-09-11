#include "AssetManager.h"
#include "Collision.h"
#include "ECS\Components.h"

extern std::vector<Vector2D> openSpaces;
Vector2D foodPos;

AssetManager::AssetManager(Manager* man) : manager(man) {}

AssetManager::~AssetManager() {}

void AssetManager::CreateProjectile(Vector2D pos, int speed, Vector2D vel, std::string id) {
    auto& projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 2);
    projectile.addComponent<ProjectileComponent>(speed, vel);
    projectile.addComponent<SpriteComponent>(id);
    projectile.addComponent<ColliderComponent>(id);
    projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::SpawnFood(std::string id) {
    Vector2D randPos;
    do {
        int randNum = rand()%(openSpaces.size());
        randPos = openSpaces[randNum];
    } while (randPos.x == foodPos.x && randPos.y == foodPos.y);
    foodPos.x = randPos.x;
    foodPos.y = randPos.y;

    auto& food(manager->addEntity());
    food.addComponent<TransformComponent>(randPos.x*(Game::DEFAULT_WIDTH*Game::MAP_SCALE), randPos.y*(Game::DEFAULT_HEIGHT*Game::MAP_SCALE));
    food.addComponent<SpriteComponent>(id);
    food.addComponent<ColliderComponent>("food");
    food.addGroup(Game::groupFood);
}

void AssetManager::SpawnChicken(int speed) {
    Entity& enemy(manager->addEntity());
    enemy.addComponent<TransformComponent>(15.0f*Game::TILE_SIZE*Game::MAP_SCALE, 17.0f*Game::TILE_SIZE*Game::MAP_SCALE); // (15,17) represents coordinates of House in map
    enemy.getComponent<TransformComponent>().speed = speed;
    std::vector<std::string> animNames = {"Idle", "Walk"};
    std::vector<int> animFrames = {2, 4};
    enemy.addComponent<SpriteComponent>("enemyChicken", animNames, animFrames, 300);
    enemy.addComponent<ColliderComponent>("enemyChicken");
    enemy.addComponent<RandomMovementComponent>(Vector2D(0, 1));
    enemy.addGroup(Game::groupEnemies);
    enemy.addGroup(Game::groupChickens);
}

void AssetManager::SpawnCow(int speed) {
    Entity& enemy(manager->addEntity());
    enemy.addComponent<TransformComponent>(15.0f*Game::TILE_SIZE*Game::MAP_SCALE, 17.0f*Game::TILE_SIZE*Game::MAP_SCALE, 28, 18, 3);
    std::vector<std::string> animNames = {"Idle", "Walk"};
    std::vector<int> animFrames = {3, 2};
    enemy.addComponent<SpriteComponent>("enemyCow", animNames, animFrames, 500);
    enemy.addComponent<ColliderComponent>("enemyCow");
    enemy.addComponent<RandomMovementComponent>(Vector2D(0, 1));
    enemy.addGroup(Game::groupEnemies);
}

void AssetManager::AddTexture(std::string id, const char* path) {
    textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id) {
    return textures[id];
}