#include "graphics.h"
#include "defs.h"
#include "game.h"
#include "obstacle.h"
#include <ctime>

int main(int argc, char* argv[]) {
    srand(time(0));
    Graphics graphics; graphics.init();
    setupGame(graphics);

    while (!isQuit()) {
        handleEvents();
        if (isPlaying()) {
            updateGame();
        }
        renderGame(graphics);
        SDL_Delay(50);
    }

    cleanupGame(graphics);
    return 0;
}
