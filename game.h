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

 enum class GameState  { MENU, PLAY, PAUSE, GAMEOVER, TTR };
 enum class MusicState {PAUSE_MUSIC,PLAY_MUSIC,NONE};
 enum class FlashCar   { ON , OFF };

 GameState gameState;
 MusicState musicState;
 FlashCar flashCar;

 bool quit = false;
 int score = 0;

 Mouse player;
 ScrollingBackground bg;
 Sprite car1, car_left1, car_right1;
 Sprite* currentCar1;
 SpriteBoom Boom;
 SpriteBoom* Boomm;

 SDL_Texture* menuTexture;
 SDL_Texture* ttrTexture;
 SDL_Texture* pauseTexture;
 SDL_Texture* ppTexture;
 SDL_Texture* gameoverTexture;
 SDL_Texture* Flash;
 SDL_Texture* flash_left;
 SDL_Texture* flash_right;
 SDL_Texture* flash;
 SDL_Texture* musicTexture;

 TTF_Font* gameFont;
 TTF_Font* EndGameFont;

 Mix_Music* gMusic;
 Mix_Music* gIntro;
 Mix_Chunk* click;
 Mix_Chunk* boom;
 Mix_Chunk* car_horn;
 static Mix_Music* currentMusic = nullptr;

 void setupGame(Graphics& graphics) {
    bg.setTexture(graphics.loadTexture(BACKGROUND_IMG));
    menuTexture = graphics.loadTexture("graphics/background/menu.png");
    ttrTexture  = graphics.loadTexture("graphics/background/ttr.png");
    pauseTexture  = graphics.loadTexture("graphics/background/pause.png");
    ppTexture  = graphics.loadTexture("graphics/background/point_pause.png");
    gameoverTexture = graphics.loadTexture("graphics/background/gameover.png");
    musicTexture = graphics.loadTexture("graphics/out/music.png");
    flash_left = graphics.loadTexture("graphics/out/flash_left.png");
    flash = graphics.loadTexture("graphics/out/flash.png");
    flash_right = graphics.loadTexture("graphics/out/flash_right.png");

    Flash=flash;

    car1.init(graphics.loadTexture(CAR_SPRITE_FILE), CAR_FRAMES, CAR_CLIPS);
    car_left1.init(graphics.loadTexture(CAR_SPRITE_FILE_LEFT), CAR_FRAMES, CAR_CLIPS);
    car_right1.init(graphics.loadTexture(CAR_SPRITE_FILE_RIGHT), CAR_FRAMES, CAR_CLIPS);
    currentCar1 = &car1;
    Boom.init(graphics.loadTexture("graphics/out/no.png"), BOOM_FRAMES, BOOM_CLIPS);
    Boomm=&Boom;

    gMusic = graphics.loadMusic("music/play/RunningAway1.mp3");
    gIntro = graphics.loadMusic("music/menu/intro.mp3");
    click = graphics.loadSound("music/click.mp3");
    boom = graphics.loadSound("music/play/boom.mp3");
    car_horn = graphics.loadSound("music/play/car_horn.mp3");

    gameFont = graphics.loadFont(Game_Font, 28);
    EndGameFont = graphics.loadFont(Game_Font,80);

    initObstacles(graphics, 8);
    loadObstacleTextures(graphics);

    flashCar = FlashCar::OFF;

    musicState = MusicState::PLAY_MUSIC;
    gameState = GameState::MENU;
    graphics.play(gIntro);
    graphics.setMusicVolume(45);
    graphics.setSoundVolume(boom,70);
    graphics.setSoundVolume(click,20);
}

 void controlMusic(Graphics& graphics) {

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
    }
    if (musicState==MusicState::PAUSE_MUSIC){
        Mix_HaltMusic();
        currentMusic = nullptr;
    }
}

 void resetGame(Graphics& graphics) {
    player = Mouse();
    currentCar1 = &car1;
    score=0;
    clearObstacles();
    initObstacles(graphics, 5);
}

 void handleEvents(Graphics& graphics) {
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
            if (e.key.keysym.sym == SDLK_f && gameState == GameState :: PLAY) {
                flashCar = (flashCar == FlashCar::ON) ? FlashCar::OFF : FlashCar::ON;
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            graphics.plays(click);

            if (mouseX >= 640 && mouseX <= 700 && mouseY >= 0 && mouseY <= 60 && musicState == MusicState::PAUSE_MUSIC) {
                musicState = MusicState::PLAY_MUSIC;
            }
            else if (mouseX >= 580 && mouseX <= 640 && mouseY >= 0 && mouseY <= 60 && musicState == MusicState::PLAY_MUSIC) {
                musicState = MusicState::PAUSE_MUSIC;
                }

            switch (gameState) {
                case GameState::PLAY:
                    if (mouseX >= 320 && mouseX <= 380 && mouseY >= 0 && mouseY <= 60) {
                        gameState = GameState::PAUSE;
                    }
                    break;

                case GameState::MENU:
                    if (mouseX >= 269 && mouseX <= 429 && mouseY >= 393 && mouseY <= 454) {
                        resetGame(graphics);
                        gameState = GameState::PLAY;
                    } else if (mouseX >= 133 && mouseX <= 566 && mouseY >= 488 && mouseY <= 578) {
                        gameState = GameState::TTR;
                    }
                    break;

                case GameState::TTR:
                    if (mouseX >= 0 && mouseX <= 93 && mouseY >= 0 && mouseY <= 62) {
                        gameState = GameState::MENU;
                    }
                    break;

            case GameState::PAUSE:
                    if (mouseX >= 180 && mouseX <= 519 && mouseY >= 118 && mouseY <= 258) {
                        gameState = GameState::PLAY;
                    } else if (mouseX >= 215 && mouseX <= 481 && mouseY >= 278 && mouseY <= 416) {
                        resetGame(graphics);
                    gameState = GameState::PLAY;
                    } else if (mouseX >= 0 && mouseX <= 590 && mouseY >= 459 && mouseY <= 579) {
                        gameState = GameState::MENU;
                    }
                    break;

            case GameState::GAMEOVER:
                    if (mouseX >= 306 && mouseX <= 393 && mouseY >= 488 && mouseY <= 542) {
                        resetGame(graphics);
                        gameState = GameState::MENU;
                    }
                    if (mouseX >= 306 && mouseX <= 393 && mouseY >= 566 && mouseY <= 619) {
                        resetGame(graphics);
                        gameState = GameState::PLAY;
                    }
                    break;
            }
        }
    }
}

 void updateGame(Graphics& graphics) {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if      (keys[SDL_SCANCODE_A]||keys[SDL_SCANCODE_LEFT]) { player.turnWest(); currentCar1 = &car_left1; Flash=flash_left; }
    else if (keys[SDL_SCANCODE_D]||keys[SDL_SCANCODE_RIGHT]) { player.turnEast(); currentCar1 = &car_right1; Flash=flash_right; }
    else if (keys[SDL_SCANCODE_W]||keys[SDL_SCANCODE_UP]) { player.turnNorth(); currentCar1 = &car1; Flash=flash; }
    else if (keys[SDL_SCANCODE_S]||keys[SDL_SCANCODE_DOWN]) { player.turnSouth(); currentCar1 = &car1; Flash=flash; }
    else                                                    { player.stop(); currentCar1 = &car1;Flash=flash; }

    player.move();
    bg.scroll(12);
    updateObstacles(8);

    if (checkPlayerCollision(player.getRect())){
        graphics.plays(boom);
        gameState = GameState::GAMEOVER;
    }
}

 void renderGame(Graphics& graphics) {
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

        if (gameState == GameState::PLAY) {
            currentCar1->tick();
        }
        if(gameState==GameState::PLAY||gameState==GameState::PAUSE) graphics.render_car(player.x, player.y, *currentCar1);
        graphics.renderTexture(ppTexture, 0, 0);
        renderObstacles(graphics);

        if (gameState == GameState::PAUSE || gameState == GameState::GAMEOVER) {

            if (gameState == GameState::GAMEOVER) {
                Boomm ->tick();
                graphics.render_boom(player.x-50,player.y-50,*Boomm);
                flashCar=FlashCar::OFF;
            }

            SDL_SetRenderDrawBlendMode(graphics.renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(graphics.renderer, 0, 0, 0, 900);
            SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(graphics.renderer, &overlay);


            if (gameState == GameState::PAUSE)graphics.renderTexture(pauseTexture, 0, 0);
            else graphics.renderTexture(gameoverTexture, 0, 0);
        }
        if(gameState!=GameState::GAMEOVER){
            SDL_Color white = {255, 255, 255};
            std::string scoreText = "Score: " + std::to_string(score);
            SDL_Texture* textTexture = graphics.renderText(scoreText.c_str(), gameFont, white);
            graphics.renderTexture(textTexture, 10, 10);
            SDL_DestroyTexture(textTexture);
        }else{
            SDL_Color white = {255,255,255};
            std::string scoreText = "SCORE : " + std::to_string(score);
            SDL_Texture* textTexture = graphics.renderText(scoreText.c_str(), EndGameFont, white);
            graphics.renderTexture(textTexture, 174,200 );
            SDL_DestroyTexture(textTexture);

        }
    }
    if(flashCar==FlashCar::ON && gameState==GameState::PLAY){

        graphics.renderTexture(Flash,player.x-80,player.y-77);
    }
    if(gameState!=GameState::TTR)graphics.renderTexture(musicTexture,0,0);
    graphics.presentScene();
}

 bool isQuit() { return quit; }
 bool isPlaying() { return gameState == GameState::PLAY; }

 void cleanupGame(Graphics& graphics) {
    clearObstacles();
    freeObstacleTextures();
    SDL_DestroyTexture(menuTexture);
    Mix_FreeMusic(gMusic);
    Mix_FreeMusic(gIntro);
    Mix_FreeChunk(click);
}

#endif // GAME_H
