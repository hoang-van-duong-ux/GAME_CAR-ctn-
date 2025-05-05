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
        handleEvents(graphics);

        if (isPlaying()) {
            updateGame(graphics);
        }
        renderGame(graphics);

        SDL_Delay(25);
    }
    cleanupGame(graphics);
    return 0;
}

