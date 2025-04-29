#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>
#include <vector>
#include "graphics.h"
#include <ctime>


// Vật cản
struct Obstacle {
    int x, y, speed;
    Sprite sprite;

    void init(int _x, int _y, int _speed, SDL_Texture* texture, int frames, const int clips[][4]) {
        x = _x; y = _y; speed = _speed;
        sprite.init(texture, frames, clips);
    }

    void move() { y += speed; }

    SDL_Rect getRect() const {
        //const SDL_Rect* clip = sprite.getCurrentClip();
        return {x, y, 50, 70};
    }
};

// Danh sách vật cản và quản lý
inline std::vector<Obstacle> obstacles;
inline const char* OBSTACLE_IMAGES[] = {
    "graphics/button/oto_canh_sat.png",
    "graphics/button/thung.png",
    "graphics/button/oto_con.png",
    "graphics/button/oto_tải.png"
};
inline int obsLanes[] = {65, 125, 200, 260, 340, 400, 473, 535};

inline void initObstacles(Graphics& graphics, int num) {
    obstacles.clear();

    for (int i = 0; i < num; ++i) {
        Obstacle obs;
        SDL_Texture* tex = graphics.loadTexture(OBSTACLE_IMAGES[i % 4]);

        bool valid;
        int x, y;

        do {
            x = obsLanes[rand() % 8];
            y = -(rand() % 150 + i * 150);
            obs.init(x, y, 10 + num, tex, CAR_FRAMES, CAR_CLIPS);

            SDL_Rect rect = obs.getRect();
            valid = true;

           for (const auto& other : obstacles) {
                SDL_Rect otherRect = other.getRect();

                if (SDL_HasIntersection(&rect, &otherRect)) {
                    valid = false;
                    break;
                    }

                if (abs(rect.y - otherRect.y) < 300 && rect.x == otherRect.x) {
                    valid = false;
                    break;
                }

            }

        } while (!valid);

        obstacles.push_back(obs);
    }
}


inline void updateObstacles() {
    for (auto& obs : obstacles) {
        obs.move();
        obs.sprite.tick();
        if (obs.y > SCREEN_HEIGHT) {
            obs.y = -rand()%300;
            obs.x = obsLanes[rand()%8];
        }
    }
}

inline void renderObstacles(Graphics& graphics) {
    for (auto& obs : obstacles)
        graphics.render_car(obs.x, obs.y, obs.sprite);
}

inline void clearObstacles() {
    for (auto& obs : obstacles)
        SDL_DestroyTexture(obs.sprite.texture);
    obstacles.clear();
}

inline bool checkPlayerCollision(const SDL_Rect& playerRect) {
    for (auto& obs : obstacles) {
        SDL_Rect obsRect = obs.getRect();
        if (SDL_HasIntersection(&playerRect, &obsRect))
            return true;
    }
    return false;
}

#endif // OBSTACLE_H
