#ifndef GAME_H
#define GAME_H

#include <bits/stdc++.h>
#include "defs.h"
#include <SDL.h>
#include "graphics.h"
#include "obstacle.h"

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

enum class GameState { MENU, PLAY, PAUSE, GAMEOVER, TTR };
enum class MusicState {PAUSE_MUSIC,PLAY_MUSIC};

inline GameState gameState;
inline MusicState musicState;
inline bool quit = false;
inline Mouse player;
inline ScrollingBackground bg;
inline Sprite car, car_left, car_right;
inline Sprite* currentCar;
inline SpriteBoom Boom;
inline SpriteBoom* Boomm;

inline SDL_Texture* menuTexture;
inline SDL_Texture* ttrTexture;
inline SDL_Texture* pauseTexture;
inline SDL_Texture* ppTexture;
inline SDL_Texture* gameoverTexture;

inline Mix_Music* gMusic;
inline Mix_Music* gIntro;
inline Mix_Chunk* click;
inline Mix_Chunk* boom;
inline Mix_Chunk* car_horn;
inline SDL_Texture* musicTexture;
inline  static Mix_Music* currentMusic = nullptr;


inline void setupGame(Graphics& graphics) {
    bg.setTexture(graphics.loadTexture(BACKGROUND_IMG));
    menuTexture = graphics.loadTexture("graphics/background/menu.png");
    ttrTexture  = graphics.loadTexture("graphics/background/ttr.png");
    pauseTexture  = graphics.loadTexture("graphics/background/pause.png");
    ppTexture  = graphics.loadTexture("graphics/background/point_pause.png");
    gameoverTexture = graphics.loadTexture("graphics/background/gameover.png");
    musicTexture=graphics.loadTexture("graphics/out/music.png");

    car.init(graphics.loadTexture(CAR_SPRITE_FILE), CAR_FRAMES, CAR_CLIPS);
    car_left.init(graphics.loadTexture("graphics/player/car_left.png"), CAR_FRAMES, CAR_CLIPS);
    car_right.init(graphics.loadTexture("graphics/player/car_right.png"), CAR_FRAMES, CAR_CLIPS);
    currentCar = &car;
    Boom.init(graphics.loadTexture("graphics/out/no.png"), BOOM_FRAMES, BOOM_CLIPS);
    Boomm=&Boom;

    gMusic = graphics.loadMusic("music/play/RunningAway1.mp3");
    gIntro = graphics.loadMusic("music/menu/intro.mp3");
    click = graphics.loadSound("music/click.mp3");
    boom = graphics.loadSound("music/play/boom.mp3");
    car_horn = graphics.loadSound("music/play/car_horn.mp3");

    initObstacles(graphics, 8);
    loadObstacleTextures(graphics);

    musicState = MusicState::PLAY_MUSIC;
    gameState = GameState::MENU;
    graphics.play(gIntro);
    graphics.setMusicVolume(45);
    graphics.setSoundVolume(boom, 128);
}

inline void controlMusic(Graphics& graphics) {

    Mix_Music* desiredMusic = nullptr;
    if(musicState==MusicState::PLAY_MUSIC){
        if (gameState == GameState::MENU || gameState == GameState::TTR)
            desiredMusic = gIntro;
        else
            desiredMusic = gMusic;



        if (currentMusic != desiredMusic) {
            Mix_HaltMusic();
            graphics.play(desiredMusic);
            currentMusic = desiredMusic;
        }
    }else if (musicState==MusicState::PAUSE_MUSIC) Mix_HaltMusic();
}

inline void resetGame(Graphics& graphics) {
    player = Mouse();
    currentCar = &car;

    clearObstacles();
    initObstacles(graphics, 5);
}

inline void handleEvents(Graphics& graphics) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) quit = true;

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE && gameState == GameState::PLAY) {
                gameState = GameState::PAUSE;
            }
            if (e.key.keysym.sym == SDLK_v && gameState == GameState::PLAY) {
                graphics.plays(car_horn);
            }

        }

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            graphics.plays(click);

            if (mouseX >= 640 && mouseX <= 700 && mouseY >= 0 && mouseY <= 60 && musicState==MusicState::PLAY_MUSIC){
                musicState = MusicState ::PAUSE_MUSIC;
            }

            if (mouseX >= 640 && mouseX <= 700 && mouseY >= 0 && mouseY <= 60 && musicState==MusicState::PAUSE_MUSIC){
                musicState = MusicState ::PLAY_MUSIC;
            }

            if(gameState==GameState::PLAY){
               if (mouseX >= 320 && mouseX <= 380 && mouseY >= 0 && mouseY <= 60 ) {
                    gameState=GameState::PAUSE;
               }

            }

            if (gameState == GameState::MENU) {
                if (mouseX >= 269 && mouseX <= 429 && mouseY >= 259 && mouseY <= 320) {
                    resetGame(graphics);
                    gameState = GameState::PLAY;
                }
                else if (mouseX >= 133 && mouseX <= 566 && mouseY >= 354 && mouseY <= 444) {
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

            if (gameState == GameState::GAMEOVER) {

                if (mouseX >= 306 && mouseX <= 393 && mouseY >= 488 && mouseY <= 542) {
                    resetGame(graphics);
                    gameState = GameState::MENU;
                }
            }
        }
    }
}

inline void updateGame(Graphics& graphics) {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A]) { player.turnWest(); currentCar = &car_left; }
    else if (keys[SDL_SCANCODE_D]) { player.turnEast(); currentCar = &car_right; }
    else if (keys[SDL_SCANCODE_W]) { player.turnNorth(); currentCar = &car; }
    else if (keys[SDL_SCANCODE_S]) { player.turnSouth(); currentCar = &car; }
    else { player.stop(); currentCar = &car; }

    player.move();
    bg.scroll(12);
    updateObstacles();

    if (checkPlayerCollision(player.getRect())){
        graphics.plays(boom);
        gameState = GameState::GAMEOVER;
    }
}

inline void renderGame(Graphics& graphics) {
    controlMusic(graphics);
    graphics.prepareScene();

    if (gameState == GameState::MENU) {
        graphics.renderTexture(menuTexture, 0, 0);
    }

    else if (gameState == GameState::TTR) {
        graphics.renderTexture(ttrTexture, 0, 0);
    }

    else if (gameState == GameState::PLAY || gameState == GameState::PAUSE || gameState == GameState::GAMEOVER) {

        graphics.render_background(bg);
        if (gameState == GameState::PLAY) currentCar->tick();
        if(gameState==GameState::PLAY||gameState==GameState::PAUSE) graphics.render_car(player.x, player.y, *currentCar);
        graphics.renderTexture(ppTexture, 0, 0);
        renderObstacles(graphics);

        if (gameState == GameState::PAUSE || gameState == GameState::GAMEOVER) {
            if (gameState == GameState::GAMEOVER) {

                Boomm ->tick();
                graphics.render_boom(player.x-50,player.y-50,*Boomm);
            }
            SDL_SetRenderDrawBlendMode(graphics.renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(graphics.renderer, 0, 0, 0, 120);
            SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(graphics.renderer, &overlay);

            if (gameState == GameState::PAUSE)graphics.renderTexture(pauseTexture, 0, 0);
            else graphics.renderTexture(gameoverTexture, 0, 0);
        }
    }
    graphics.renderTexture(musicTexture,0,0);
    graphics.presentScene();
}

inline bool isQuit() { return quit; }
inline bool isPlaying() { return gameState == GameState::PLAY; }

inline void cleanupGame(Graphics& graphics) {
    clearObstacles();
    freeObstacleTextures();
    SDL_DestroyTexture(menuTexture);
    Mix_FreeMusic(gMusic);
    Mix_FreeMusic(gIntro);
    Mix_FreeChunk(click);
}

#endif // GAME_H
