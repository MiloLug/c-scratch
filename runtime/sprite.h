#ifndef SPRITE_H
#define SPRITE_H

#include "include_sdl.h"
#include <filesystem>
#include <vector>
#include <thread>
#include <cmath>


const std::filesystem::path spritesBaseDirectory = "sprites/";


class Sprite {
public:
    const char * name;
    SDL_FRect pos;
    double direction;
    int costumeNumber;
    double size;
    int visible;
    int layerOrder;
    std::vector<SDL_Texture*> textures = {};
    std::atomic<bool> canMove = true;


    inline void setX(double x) {
        #ifdef ENABLE_TURBO
            this->pos.x = (WIDTH - this->pos.w) / 2.0 + x;
        #else
            if (canMove) {
                this->pos.x = (WIDTH - this->pos.w) / 2.0 + x;
                canMove = false;
            }
        #endif
    }

    inline void setY(double y) {
        #ifdef ENABLE_TURBO
            this->pos.y = (HEIGHT - this->pos.h) / 2.0 + y;
        #else
            if (canMove) {
                this->pos.y = (HEIGHT - this->pos.h) / 2.0 + y;
                canMove = false;
            }
        #endif
    }

    inline void goXY(double x, double y) {
        #ifdef ENABLE_TURBO
            pos.x = (WIDTH - pos.w) / 2.0 + x;
            pos.y = (HEIGHT - pos.h) / 2.0 + y;
        #else
            if (canMove) {
                pos.x = (WIDTH - pos.w) / 2.0 + x;
                pos.y = (HEIGHT - pos.h) / 2.0 + y;
                canMove = false;
            }
        #endif
    }

    inline void changeX(double value) {
        #ifdef ENABLE_TURBO
            pos.x += value;
        #else
            if (canMove) {
                pos.x += value;
                canMove = false;
            }
        #endif
    }

    inline void changeY(double value) {
        #ifdef ENABLE_TURBO
            pos.y -= value;
        #else
            if (canMove) {
                pos.y -= value;
                canMove = false;
            }
        #endif
    }

    inline void turnRight(double value) {
        #ifdef ENABLE_TURBO
            direction += value;
            direction = fmod(direction, 360.0);
        #else
            if (canMove) {
                direction += value;
                direction = fmod(direction, 360.0);
                canMove = false;
            }
        #endif
    }

    inline void turnLeft(double value) {
        #ifdef ENABLE_TURBO
            direction -= value;
            direction = fmod(direction, 360.0);
        #else
            if (canMove) {
                direction -= value;
                direction = fmod(direction, 360.0);
                canMove = false;
            }
        #endif
    }

    inline void point(double value) {
        #ifdef ENABLE_TURBO
            direction = fmod(value - 90.0, 360.0);
        #else
            if (canMove) {
                direction = fmod(value - 90.0, 360.0);
                canMove = false;
            }
        #endif
    }

    inline void move(double value) {
        #ifdef ENABLE_TURBO
            pos.x += value * cos(direction * M_PI / 180.0);
            pos.y += value * sin(direction * M_PI / 180.0);
        #else
            if (canMove) {
                pos.x += value * cos(direction * M_PI / 180.0);
                pos.y += value * sin(direction * M_PI / 180.0);
                canMove = false;
            }
        #endif
    }


    inline void init(SDL_Renderer * renderer) {
        const std::filesystem::path spritePath = spritesBaseDirectory / name;

        for (auto const& dir_entry : std::filesystem::directory_iterator{ spritePath / "costumes" })
        {
            textures.push_back(IMG_LoadTexture(renderer, dir_entry.path().string().c_str()));
        }
    }

    inline SDL_Texture * getCostumeTexture() const {
        return this->textures[this->costumeNumber];
    }
};


#endif
