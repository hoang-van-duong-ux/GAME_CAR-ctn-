#ifndef GAME_H
#define GAME_H

#include <bits/stdc++.h>
#include "defs.h"
#include <SDL.h>
#include "defs.h"
#include "graphics.h"
#include "obstacle.h"

// ==== Mouse player ====
enum class Direction { None, North, South, West, East };

struct Mouse {
    int x = 300, y = 600, dx = 0, dy = 0, speed = INITIAL_SPEED;
    Direction dir = Direction::None;

    void move() {
        x += dx; y += dy;
        if (x < 70) x = 70;
        if (x + PLAYER_WIDTH > SCREEN_WIDTH - 70) x = SCREEN_WIDTH - PLAYER_WIDTH - 70;
        if (y < 0) y = 0;
        if (y + PLAYER_HEIGHT > SCREEN_HEIGHT) y = SCREEN_HEIGHT - PLAYER_HEIGHT;
    }

    void turnNorth() { dx = 0; dy = -speed; dir = Direction::North; }
    void turnSouth() { dx = 0; dy = speed; dir = Direction::South; }
    void turnWest()  { dx = -speed; dy = 0; dir = Direction::West; }
    void turnEast()  { dx = speed; dy = 0; dir = Direction::East; }
    void stop() { dx = dy = 0; dir = Direction::None; }

    SDL_Rect getRect() const { return {x, y, 50, 70}; }
};

// ==== Game System ====

enum class GameState { MENU, PLAY, PAUSE, GAMEOVER,TTR };

// Các biến toàn cục
inline GameState gameState;
inline bool quit = false;
inline Mouse player;
inline ScrollingBackground bg;
inline Sprite car, car_left, car_right;
inline Sprite* currentCar;
inline SDL_Texture* menuTexture;
inline SDL_Texture* ttrTexture;
inline SDL_Texture* pauseTexture;
inline SDL_Texture* ppTexture;
inline SDL_Texture* gameoverTexture;


// ==== Khởi tạo game ====
inline void setupGame(Graphics& graphics) {

    bg.setTexture(graphics.loadTexture(BACKGROUND_IMG));
    menuTexture = graphics.loadTexture("graphics/background/menu.png");
    ttrTexture  = graphics.loadTexture("graphics/background/ttr.png");
    pauseTexture  = graphics.loadTexture("graphics/background/pause.png");
    ppTexture  = graphics.loadTexture("graphics/background/point_pause.png");
    gameoverTexture = graphics.loadTexture("graphics/background/gameover.png");

    car.init(graphics.loadTexture(CAR_SPRITE_FILE), CAR_FRAMES, CAR_CLIPS);
    car_left.init(graphics.loadTexture("graphics/player/car_left.png"), CAR_FRAMES, CAR_CLIPS);
    car_right.init(graphics.loadTexture("graphics/player/car_right.png"), CAR_FRAMES, CAR_CLIPS);
    currentCar = &car;

    initObstacles(graphics,8);
    loadObstacleTextures(graphics);


    gameState = GameState::MENU;
}

inline void resetGame(Graphics& graphics) {
    player = Mouse();
    currentCar = &car;

    clearObstacles();
    initObstacles(graphics, 5);
}

// ==== Xử lý sự kiện (phím bấm) ====
inline void handleEvents(Graphics& graphics) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) quit = true;

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE && gameState == GameState::PLAY) {
                gameState = GameState::PAUSE;
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            if (gameState == GameState::MENU) {
                if (mouseX >= 269 && mouseX <= 429 && mouseY >= 259 && mouseY <= 320) {
                    resetGame(graphics);
                    gameState = GameState::PLAY;
                }
                else if (mouseX >= 133 && mouseX <= 566 && mouseY >= 354 && mouseY <= 413) {
                    gameState = GameState::TTR;
                }
            }

            if (gameState == GameState::TTR) {
                if (mouseX >= 0 && mouseX <= 93 && mouseY >= 0 && mouseY <= 62) {
                    gameState = GameState::MENU;
                }
            }
            if (gameState == GameState::PAUSE) {
                if (mouseX >= 180 && mouseX <= 519 && mouseY >= 118 && mouseY <= 258) {
                    gameState = GameState::PLAY;
                }
                if (mouseX >= 215 && mouseX <= 481 && mouseY >= 278 && mouseY <= 416) {
                    resetGame(graphics);
                    gameState = GameState::PLAY;
                }
                if (mouseX >= 0 && mouseX <= 590 && mouseY >= 459 && mouseY <= 579) {
                    gameState = GameState::MENU;
                }
            }
            if (gameState == GameState::GAMEOVER ) {
                 if (mouseX >= 306 && mouseX <= 393 && mouseY >= 488 && mouseY <= 542 ){
                    resetGame(graphics);
                    gameState = GameState::MENU;
                    }
            }
        }
    }
}

// ==== Cập nhật di chuyển ====
inline void updateGame() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A]) { player.turnWest(); currentCar = &car_left; }
    else if (keys[SDL_SCANCODE_D]) { player.turnEast(); currentCar = &car_right; }
    else if (keys[SDL_SCANCODE_W]) { player.turnNorth(); currentCar = &car; }
    else if (keys[SDL_SCANCODE_S]) { player.turnSouth(); currentCar = &car; }
    else { player.stop(); currentCar = &car; }

    player.move();
    bg.scroll(12);
    updateObstacles();

    if (checkPlayerCollision(player.getRect()))
        gameState = GameState::GAMEOVER;

}

// ==== Vẽ màn hình ====
inline void renderGame(Graphics& graphics) {
    graphics.prepareScene();

    if (gameState == GameState::MENU) {

    graphics.renderTexture(menuTexture, 0, 0);
    }
    if (gameState == GameState::TTR) {

    graphics.renderTexture(ttrTexture, 0, 0);
    }

    if (gameState == GameState::PLAY || gameState == GameState::PAUSE || gameState == GameState::GAMEOVER) {

        graphics.renderTexture(ppTexture, 0, 0);
        graphics.render_background(bg);
        if (gameState == GameState::PLAY) currentCar->tick();
        graphics.render_car(player.x, player.y, *currentCar);
        renderObstacles(graphics);

        if (gameState == GameState::PAUSE) {
            SDL_SetRenderDrawBlendMode(graphics.renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(graphics.renderer, 0, 0, 0, 120);
            SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(graphics.renderer, &overlay);

            graphics.renderTexture(pauseTexture, 0, 0);
        }

        if (gameState == GameState::GAMEOVER) {
            SDL_SetRenderDrawBlendMode(graphics.renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(graphics.renderer, 0, 0, 0, 120);
            SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(graphics.renderer, &overlay);

            graphics.renderTexture(gameoverTexture, 0, 0);
        }
}


    graphics.presentScene();
}

// ==== Kiểm tra trạng thái game ====
inline bool isQuit() { return quit; }
inline bool isPlaying() { return gameState == GameState::PLAY; }

// ==== Dọn bộ nhớ khi thoát game ====
inline void cleanupGame(Graphics& graphics) {
    clearObstacles();

    freeObstacleTextures();

    SDL_DestroyTexture(menuTexture);
}

#endif // GAME_H
