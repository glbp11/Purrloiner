#pragma once

#include <map>
#include <SDL2/SDL.h>
#include "Animation.h"
#include "Components.h"
#include "../AssetManager.h"
#include "../TextureManager.h"

class SpriteComponent : public Component {
private:
    bool animated = false;
    int frames = 0;
    int speed = 100;

    SDL_Rect srcRect, destRect;
    SDL_Texture * texture;
    TransformComponent * transform;

public:
    int animIndex = 0;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    std::map<std::string, Animation> animations;

    SpriteComponent() = default;
    SpriteComponent(std::string id) {
        setTex(id);
    }

    SpriteComponent(std::string id, std::vector<std::string> names, std::vector<int> frames, int speed) {
        animated = true;
        if (names.size() == frames.size()) {
            for (int i = 0; i < names.size(); i++) {
                animations.emplace(names[i], Animation(i, frames[i], speed));
            }
        } else {
            std::cout << "names size does not correspond to frames size" << std::endl;
        }
        Play(names[0]);
        setTex(id);
    }

    ~SpriteComponent() {}

    void setTex(std::string id) {
        texture = Game::assets->GetTexture(id);
    }

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
    }

    void update() override {
        if (animated) {
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks64() / speed) % frames);
        }
        srcRect.y = animIndex * transform->height;
        destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
        destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void draw() override {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void Play(std::string animName) {
        animIndex = animations[animName].index;
        frames = animations[animName].frames;
        speed = animations[animName].speed;
    }
};