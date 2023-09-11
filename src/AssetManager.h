#pragma once

#include <map>
#include <string>
#include "ECS\ECS.h"
#include "TextureManager.h"
#include "Vector2D.h"

class AssetManager {
private:
    Manager* manager;
    std::map<std::string, SDL_Texture*> textures;

public:
    AssetManager(Manager * man);
    ~AssetManager();

    // Game Objects
    void CreateProjectile(Vector2D pos, int speed, Vector2D vel, std::string id);
    void SpawnFood(std::string id);
    void SpawnChicken(int speed);
    void SpawnCow(int speed);

    // Texture/Sprite management
    void AddTexture(std::string id, const char* path);
    SDL_Texture* GetTexture(std::string id);
};