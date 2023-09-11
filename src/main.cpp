#include "Game.h"

Game *game = nullptr;

int main (int arc, char *argv[]) {
    Uint32 frameStart;
    int frameTime;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    game = new Game();
    game->init("Survival", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Game::CAMERA_WIDTH, Game::CAMERA_HEIGHT, false);

    while(game->state == game->STATE_MENU && game->running()) {
        game->handleEvents();
        game->renderMenu();
    }

    while(game->running()){
        if (game->state == game->STATE_GAME) {
            frameStart = SDL_GetTicks64();

            game->handleEvents();
            game->update();
            game->render();

            frameTime = SDL_GetTicks64() - frameStart;

            if (frameDelay > frameTime){
                SDL_Delay(frameDelay - frameTime);
            }
        } else if (game->state == game->STATE_RESTART) {
            game->handleEvents();
            game->renderRestart();
        }
    }

    game->clean();

    return 0;
}