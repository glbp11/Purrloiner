#pragma once

#include "Game.h"

class Button {
private:
    bool pressed;
    SDL_Rect button;
    SDL_Rect srcRect;

public:
    Button(int x, int y, int w, int h);
    ~Button();
    void drawButton(SDL_Texture* tex);
    bool buttonPressed(SDL_MouseButtonEvent& e);
};