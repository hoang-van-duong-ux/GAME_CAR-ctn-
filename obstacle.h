#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "graphics.h"
#include "defs.h"

// Cấu trúc obstacle
struct Obstacle {
    int x, y, speed;
    Sprite sprite;

    void init(int _x, int _y, int _speed, SDL_Texture* texture, int frames, const int clips[][4]) {
        x = _x;
        y = _y;
        speed = _speed;
        sprite.init(texture, frames, clips);
    }

    void move() {
        y += speed;
    }

    SDL_Rect getRect() const {
        return {x, y, 50, 70};
    }
};

// ==== Danh sách vật cản ====
inline std::vector<Obstacle> obstacles;

// ==== Danh sách hình ảnh vật cản và texture ====
inline const char* OBSTACLE_IMAGES[] = {
    "graphics/button/oto_canh_sat.png",
    "graphics/button/thung.png",
    "graphics/button/oto_con.png",
    "graphics/button/oto_tải.png"
   // "graphics/button/oto_con.png",
    "graphics/button/oto_tải.png" ,
    "graphics/button/oto_con1.png"
};

inline SDL_Texture* obstacleTextures[4];

// ==== Vị trí các làn xe ====
inline int obsLanes[] = {65, 125, 200, 260, 340, 400, 473, 535};

// ==== Load texture 1 lần ====
inline void loadObstacleTextures(Graphics& graphics) {
    for (int i = 0; i < 4; ++i) {
        obstacleTextures[i] = graphics.loadTexture(OBSTACLE_IMAGES[i]);
    }
}

// ==== Giải phóng texture ====
inline void freeObstacleTextures() {
    for (int i = 0; i < 4; ++i) {
        SDL_DestroyTexture(obstacleTextures[i]);
        obstacleTextures[i] = nullptr;
    }
}

// ==== Tạo obstacle ban đầu ====
inline void initObstacles(Graphics& graphics, int num) {
    obstacles.clear();
    std::vector<int> usedLanes;

    for (int i = 0; i < num; ++i) {
        int laneIndex;
        do {
            laneIndex = rand() % 8;
        } while (std::find(usedLanes.begin(), usedLanes.end(), laneIndex) != usedLanes.end());

        usedLanes.push_back(laneIndex);
        int x = obsLanes[laneIndex];
        int y = -(rand() % 150 + i * 150);
        int speed = rand() % 5 + 5;

        Obstacle obs;
        SDL_Texture* tex = obstacleTextures[i % 4];
        obs.init(x, y, speed, tex, CAR_FRAMES, CAR_CLIPS);
        obstacles.push_back(obs);
    }
}

// ==== Cập nhật obstacle mỗi frame ====
inline void updateObstacles() {
    bool laneOccupied[8] = {false};

    for (auto& obs : obstacles) {
        obs.move();
        obs.sprite.tick();

        for (int i = 0; i < 8; ++i) {
            if (obs.x == obsLanes[i] && obs.y > -80 && obs.y < SCREEN_HEIGHT)
                laneOccupied[i] = true;
        }

        if (obs.y > SCREEN_HEIGHT) {
            // Tìm làn mới không bị chiếm
            std::vector<int> freeLanes;
            for (int i = 0; i < 8; ++i) {
                if (!laneOccupied[i])
                    freeLanes.push_back(i);
            }

            if (!freeLanes.empty()) {
                int newLane = freeLanes[rand() % freeLanes.size()];
                obs.x = obsLanes[newLane];
                obs.y = -rand() % 300;
                laneOccupied[newLane] = true;
            }
        }
    }
}

// ==== Vẽ tất cả obstacles ====
inline void renderObstacles(Graphics& graphics) {
    for (auto& obs : obstacles)
        graphics.render_car(obs.x, obs.y, obs.sprite);
}

// ==== Xoá sạch ====
inline void clearObstacles() {
    obstacles.clear();
}

// ==== Kiểm tra va chạm ====
inline bool checkPlayerCollision(const SDL_Rect& playerRect) {
    for (auto& obs : obstacles) {
        SDL_Rect obsRect = obs.getRect();
        if (SDL_HasIntersection(&playerRect, &obsRect))
            return true;
    }
    return false;
}

#endif // OBSTACLE_H
