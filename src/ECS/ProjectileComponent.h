#pragma once

#include "Components.h"
#include "ECS.h"
#include "../Game.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component {
private:
    int speed;
    Vector2D velocity;
    TransformComponent * transform;

public:
    ProjectileComponent(int s, Vector2D v) : speed(s), velocity(v) {};

    ~ProjectileComponent() {};

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        transform->speed = speed;
        transform->velocity = velocity;
    }

    void update() override {
        if (transform->position.x > Game::camera.x + Game::camera.w || 
                    transform->position.x < Game::camera.x ||
                    transform->position.y > Game::camera.y + Game::camera.h ||
                    transform->position.y < Game::camera.y) {
            entity->destroy();
        }
    }
};