#ifndef _DEFS__H
#define _DEFS__H

#define INITIAL_SPEED 11
#define PLAYER_WIDTH  100
#define PLAYER_HEIGHT 100

const int SCREEN_WIDTH = 700, SCREEN_HEIGHT = 700;
#define WINDOW_TITLE " DI XE CHU Y CAN THAN "
#define BACKGROUND_IMG "graphics//background//map1.png"

const char* Game_Font = "graphics/font/MouldyCheeseRegular-WyMWG.ttf";

const char* CAR_SPRITE_FILE = "graphics/player/car.png";
const char* CAR_SPRITE_FILE_LEFT  ="graphics/player/car_left.png";
const char* CAR_SPRITE_FILE_RIGHT ="graphics/player/car_right.png";

const int CAR_CLIPS[][4] = {
    {0,0,100,100},{100,0,100,100},{200,0,100,100},{300,0,100,100}
};
const int BOOM_CLIPS[][8] = {
    {0,0,210,160},{210,0,210,160},{420,0,210,160},{630,0,210,160},{840,0,210,160},{1050,0,210,160},{1260,0,210,160},{1470,0,210,160}
};
const int CAR_FRAMES = sizeof(CAR_CLIPS)/sizeof(int)/4;
const int BOOM_FRAMES = sizeof(BOOM_CLIPS)/sizeof(int)/8;


#endif
