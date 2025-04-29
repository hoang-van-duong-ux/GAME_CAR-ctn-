#ifndef GAME_H
#define GAME_H

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

enum class GameState { MENU, PLAY, PAUSE, GAMEOVER };

// Các biến toàn cục
inline GameState gameState;
inline bool quit = false;
inline Mouse player;
inline ScrollingBackground bg;
inline Sprite car, car_left, car_right;
inline Sprite* currentCar;
inline SDL_Texture* menuTexture;


// ==== Khởi tạo game ====
inline void setupGame(Graphics& graphics) {

    bg.setTexture(graphics.loadTexture(BACKGROUND_IMG));
    menuTexture = graphics.loadTexture("graphics/background/menu.png");

    car.init(graphics.loadTexture(CAR_SPRITE_FILE), CAR_FRAMES, CAR_CLIPS);
    car_left.init(graphics.loadTexture("graphics/player/car_left.png"), CAR_FRAMES, CAR_CLIPS);
    car_right.init(graphics.loadTexture("graphics/player/car_right.png"), CAR_FRAMES, CAR_CLIPS);
    currentCar = &car;

    initObstacles(graphics,7);

    gameState = GameState::MENU;
}

// ==== Xử lý sự kiện (phím bấm) ====
inline void handleEvents(Graphics& graphics) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) quit = true;
        else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE && gameState==GameState::PLAY) {
                if (gameState == GameState::PLAY) gameState = GameState::PAUSE;
                else if (gameState == GameState::PAUSE) gameState = GameState::PLAY;
            }
        }
    }
}


// ==== Cập nhật di chuyển ====
inline void updateGame() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT]) { player.turnWest(); currentCar = &car_left; }
    else if (keys[SDL_SCANCODE_RIGHT]) { player.turnEast(); currentCar = &car_right; }
    else if (keys[SDL_SCANCODE_UP]) { player.turnNorth(); currentCar = &car; }
    else if (keys[SDL_SCANCODE_DOWN]) { player.turnSouth(); currentCar = &car; }
    else { player.stop(); currentCar = &car; }

    player.move();
    bg.scroll(15);
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

    if(gameState==GameState::PLAY){
        graphics.render_background(bg);
        currentCar->tick();
        graphics.render_car(player.x, player.y, *currentCar);
        renderObstacles(graphics);
    }

    graphics.presentScene();
}

// ==== Kiểm tra trạng thái game ====
inline bool isQuit() { return quit; }
inline bool isPlaying() { return gameState == GameState::PLAY; }

// ==== Dọn bộ nhớ khi thoát game ====
inline void cleanupGame(Graphics& graphics) {
    clearObstacles();
    SDL_DestroyTexture(menuTexture);
}

#endif // GAME_H
