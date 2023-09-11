#pragma once

#include "Components.h"
#include "ECS.h"
#include "../Game.h"

class KeyboardController : public Component {
private:
    std::map<std::string, bool> pressing = {{"w", false}, {"a", false}, {"s", false}, {"d", false}};
    SDL_Keycode prevKey;

public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

   void update() override {
        if (Game::event.type == SDL_KEYDOWN) {
            switch (Game::event.key.keysym.sym) {
                case SDLK_w:
                    pressing["w"] = true;
                    break;
                case SDLK_a:
                    pressing["a"] = true;
                    break;
                case SDLK_s:
                    pressing["s"] = true;
                    break;
                case SDLK_d:
                    pressing["d"] = true;
                    break;
                default:
                    break;
            }
        }

        if (Game::event.type == SDL_KEYUP) {
            switch (Game::event.key.keysym.sym) {
                case SDLK_w:
                    pressing["w"] = false;
                    prevKey = SDLK_w;
                    break;
                case SDLK_a:
                    pressing["a"] = false;
                    prevKey = SDLK_a;
                    break;
                case SDLK_s:
                    pressing["s"] = false;
                    prevKey = SDLK_s;
                    break;
                case SDLK_d:
                    pressing["d"] = false;
                    prevKey = SDLK_d;
                    break;
                case SDLK_ESCAPE:
                    Game::isRunning = false;
                default:
                    break;
            }
        }

        if (pressing["w"]) {
            if (pressing["a"]) {
                transform->velocity.x = -1;
                transform->velocity.y = -1;
                sprite->Play("WalkLeft");
            } else if (pressing["s"]) {
                transform->velocity.y = 0;
            } else if (pressing["d"]) {
                transform->velocity.x = 1;
                transform->velocity.y = -1;
                sprite->Play("WalkRight");
            } else {
                transform->velocity.x = 0;
                transform->velocity.y = -1;
                sprite->Play("WalkUp");
            }
        } else if (pressing["a"]) {
            if (pressing["s"]) {
                transform->velocity.x = -1;
                transform->velocity.y = 1;
                sprite->Play("WalkLeft");
            } else if (pressing["d"]) {
                transform->velocity.x = 0;
            } else {
                transform->velocity.x = -1;
                transform->velocity.y = 0;
                sprite->Play("WalkLeft");
            }
        } else if (pressing["s"]) {
            if (pressing["d"]) {
                transform->velocity.x = 1;
                transform->velocity.y = 1;
                sprite->Play("WalkRight");
            } else {
                transform->velocity.x = 0;
                transform->velocity.y = 1;
                sprite->Play("WalkDown");
            }
        } else if (pressing["d"]) {
            transform->velocity.x = 1;
            transform->velocity.y = 0;
            sprite->Play("WalkRight");
        } else {
            transform->velocity.x = 0;
            transform->velocity.y = 0;
            if (prevKey == SDLK_w) {
                sprite->Play("IdleBack");
            } else if (prevKey == SDLK_a) {
                sprite->Play("IdleLeft");
            } else if (prevKey == SDLK_d) {
                sprite->Play("IdleRight");
            } else {
                sprite->Play("IdleFront");
            }
        }
    }

    void ResetPressing() {
        pressing["w"] = false;
        pressing["a"] = false;
        pressing["s"] = false;
        pressing["d"] = false;
    }
};