#pragma once

#include "Components.h"
#include "../Game.h"
#include "../Vector2D.h"

struct TransformComponent : public Component {
public:
    Vector2D position;
    Vector2D velocity;

    int width = Game::DEFAULT_WIDTH;
    int height = Game::DEFAULT_HEIGHT;
    int scale = Game::DEFAULT_SCALE;
    int speed = 1;

    TransformComponent() {
        position.Zero();
    }

    TransformComponent(float x, float y) {
        position.x = x;
        position.y = y;
    }

    TransformComponent(float x, float y, int s) {
        position.x = x;
        position.y = y;
        scale = s;
    }

    TransformComponent(float x, float y, int w, int h, int s) {
        position.x = x;
        position.y = y;
        width = w;
        height = h;
        scale = s;
    }

    void init() override {
        velocity.Zero();
    }

    void update() override {
        Vector2D normVector = velocity.normalize();
        position.x += normVector.x * speed;
        position.y += normVector.y * speed;
    }
};