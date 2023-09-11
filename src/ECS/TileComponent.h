#pragma once

#include <SDL2/SDL.h>
#include "ECS.h"

class TileComponent : public Component {
public:
    Vector2D position;
    SDL_Rect srcRect, destRect;
    SDL_Texture* texture;

    TileComponent() = default;

    ~TileComponent() {
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string id) {
        texture = Game::assets->GetTexture(id);
        
        position.x = xpos;
        position.y = ypos;

        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = srcRect.h = tsize;

        destRect.w = destRect.h = tsize * tscale;

    }

    void update() override {
        destRect.x = static_cast<int>(position.x - Game::camera.x);
        destRect.y = static_cast<int>(position.y - Game::camera.y);
    }

    void draw() override {
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }
};