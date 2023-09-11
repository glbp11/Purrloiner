#include "Button.h"

Button::Button(int x, int y, int w, int h) {
    button.x = x;
    button.y = y;
    button.w = w;
    button.h = h;
    srcRect.w = 96;
    srcRect.h = 27;
}

Button::~Button() {}

void Button::drawButton(SDL_Texture* tex) {
    if (!pressed) {
        SDL_RenderCopy(Game::renderer, tex, &srcRect, &button);
    } else {
        srcRect.x = srcRect.w;
        SDL_RenderCopy(Game::renderer, tex, &srcRect, &button);
    }
}

bool Button::buttonPressed(SDL_MouseButtonEvent& e) {
    if (e.button == SDL_BUTTON_LEFT &&
            e.x >= button.x &&
            e.x <= (button.x + button.w) &&
            e.y >= button.y &&
            e.y <= (button.y + button.h)) {
        pressed = true;
        return true;
    } else {
        return false;
    }
}