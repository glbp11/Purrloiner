#include "Collision.h"
#include "ECS/ColliderComponent.h"
#include "Vector2D.h"

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {
    if (recA.x + recA.w > recB.x &&
        recB.x + recB.w > recA.x &&
        recA.y + recA.h > recB.y &&
        recB.y + recB.h > recA.y) {
        return true;
    }
    return false;
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
    if(AABB(colA.collider, colB.collider)) {
        return true;
    }
    return false;
}

void Collision::collisionResolution(Entity* entity, const Vector2D &direction, const Vector2D &position, const SDL_Rect& collider, const SDL_Rect& entityCollider) {
    if (direction.y == 0) {
        if (direction.x < 0) { // Left
            entity->getComponent<TransformComponent>().position.x = collider.x - entityCollider.w;
        } else if (direction.x > 0) { // Right
            entity->getComponent<TransformComponent>().position.x = collider.x + collider.w;
        }
    }
    else if (direction.x == 0) {
        if (direction.y < 0) { // Top
            entity->getComponent<TransformComponent>().position.y = collider.y - entityCollider.h;
        } else if (direction.y > 0) { // Bottom
            entity->getComponent<TransformComponent>().position.y = collider.y + collider.h;
        }
    }
    else {
        if (direction.x < 0 && direction.y < 0) { // Top-left
            int xOverlap = position.x + entityCollider.w - collider.x;
            int yOverlap = position.y + entityCollider.h - collider.y;
            if (xOverlap > yOverlap) {
                entity->getComponent<TransformComponent>().position.y = collider.y - entityCollider.h;
            } else if (xOverlap < yOverlap) {
                entity->getComponent<TransformComponent>().position.x = collider.x - entityCollider.w;
            }
        } else if (direction.x < 0 && direction.y > 0) { // Bottom-left
            int xOverlap = position.x + entityCollider.w - collider.x;
            int yOverlap = collider.y + collider.h - position.y;
            if (xOverlap > yOverlap) {
                entity->getComponent<TransformComponent>().position.y = collider.y + collider.h;
            } else if (xOverlap < yOverlap) {
                entity->getComponent<TransformComponent>().position.x = collider.x - entityCollider.w;
            }
        } else if (direction.x > 0 && direction.y < 0) { // Top-right
            int xOverlap = collider.x + collider.w - position.x;
            int yOverlap = position.y + entityCollider.h - collider.y;
            if (xOverlap > yOverlap) {
                entity->getComponent<TransformComponent>().position.y = collider.y - entityCollider.h;
            } else if (xOverlap < yOverlap) {
                entity->getComponent<TransformComponent>().position.x = collider.x + collider.w;
            }
        } else { // Bottom-right
            int xOverlap = collider.x + collider.w - position.x;
            int yOverlap = collider.y + collider.h - position.y;
            if (xOverlap > yOverlap) {
                entity->getComponent<TransformComponent>().position.y = collider.y + collider.h;
            } else if (xOverlap < yOverlap) {
                entity->getComponent<TransformComponent>().position.x = collider.x + collider.w;
            }
        }
    }
}