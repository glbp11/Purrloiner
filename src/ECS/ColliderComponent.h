#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "Components.h"

class ColliderComponent : public Component {
public:
    std::string tag;
    SDL_Rect collider;
    TransformComponent * transform;

    ColliderComponent(std::string t) {
        tag = t;
    }

    ColliderComponent(std::string t, int xpos, int ypos, int size) {
        tag = t;
        collider.x = xpos;
        collider.y = ypos;
        collider.w = size;
        collider.h = size;
    }

    void init() override {
        if(!entity->hasComponent<TransformComponent>()) {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override {
        if (tag != "terrain") {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
        }
    }
};