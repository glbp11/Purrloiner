#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture) {
    return IMG_LoadTexture(Game::renderer, texture);
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, 0, NULL, flip);
}