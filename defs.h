#ifndef _DEFS__H
#define _DEFS__H

#define INITIAL_SPEED 11
#define PLAYER_WIDTH  100
#define PLAYER_HEIGHT 100

const int SCREEN_WIDTH = 700, SCREEN_HEIGHT = 700;
#define WINDOW_TITLE "Hello World!"
#define BACKGROUND_IMG "graphics//background//map1.png"

const char* CAR_SPRITE_FILE = "graphics/player/car.png";


const int CAR_CLIPS[][4] = {
    {0,0,100,100},{100,0,100,100},{200,0,100,100},{300,0,100,100}
};
const int CAR_FRAMES = sizeof(CAR_CLIPS)/sizeof(int)/4;

#endif
