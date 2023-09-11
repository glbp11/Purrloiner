#pragma once

#include <SDL2/SDL.h>
#include "ECS/ECS.h"

class ColliderComponent;
class Vector2D;

class Collision {
public:
    static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
    static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
    static void collisionResolution(Entity* entity, const Vector2D &direction, const Vector2D &position, const SDL_Rect& collider, const SDL_Rect& entityCollider);
};