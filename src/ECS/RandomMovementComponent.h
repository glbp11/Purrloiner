#pragma once

#include "Components.h"
#include "../Vector2D.h"

class RandomMovementComponent : public Component {
private:
    int duration = 0;
    Vector2D velocity;
    TransformComponent *transform;
    SpriteComponent *sprite;

public:
    RandomMovementComponent(Vector2D v) : velocity(v) {};

    ~RandomMovementComponent() {};

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        transform->velocity = velocity;
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override {
        if (SDL_GetTicks64() > duration) {
            changeDirection();
            duration = SDL_GetTicks64() + rand()%5000;
        }
    }

    void changeDirection() {
        sprite->Play("Walk");
        switch (rand() % 4) {
            case 0:
                transform->velocity.y = -0.5;
                break;
            case 1:
                transform->velocity.x = -0.5;
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                break;
            case 2:
                transform->velocity.y = 0.5;                
                break;
            case 3:
                transform->velocity.x = 0.5;                
                sprite->spriteFlip = SDL_FLIP_NONE;
                break;
        }
    }
};