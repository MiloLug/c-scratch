#ifndef SPRITE_H
#define SPRITE_H

#include <filesystem>
#include <vector>
#include <thread>
#include <cmath>
#include "include_sdl.h"
#include "../config.h"
#include <cwchar>


const std::filesystem::path spritesBaseDirectory = L"sprites/";


class Sprite {
protected:
public:
    const wchar_t * name;
    SDL_FRect pos;
    float direction;
    int costumeNumber;
    float size;
    int visible;
    int layerOrder;
    float x;
    float y;
    std::vector<SDL_Texture*> textures = {};

    inline void setX(float _x) {
        pos.x = (WINDOW_WIDTH - this->pos.w) / 2.0 + _x;
        x = _x;
    }

    inline void setY(float _y) {
        pos.y = (WINDOW_HEIGHT - this->pos.h) / 2.0 + _y;
        y = _y;
    }

    inline void goXY(float _x, float _y) {
        pos.x = (WINDOW_WIDTH - pos.w) / 2.0 + x;
        pos.y = (WINDOW_HEIGHT - pos.h) / 2.0 + y;
        x = _x;
        y = _y;
    }

    inline void changeX(float offset) {
        pos.x += offset;
        x += offset;
    }

    inline void changeY(float offset) {
        pos.y -= offset;
        y -= offset;
    }

    inline void turnRight(float angle) {
        direction += angle;
        direction = fmod(direction, 360.0);
    }

    inline void turnLeft(float angle) {
        direction -= angle;
        direction = fmod(direction, 360.0);
    }

    inline void point(float angle) {
        direction = fmod(angle - 90.0, 360.0);
    }

    inline void move(float distance) {
        float dX = distance * cos(direction * M_PI / 180.0);
        float dY = distance * sin(direction * M_PI / 180.0);

        pos.x += dX;
        pos.y += dY;
        x += dX;
        y += dY;
    }

    inline float getDirection() {
        return fmod(direction + 90.0, 360.0);
    }


    inline void init(SDL_Renderer * renderer) {
        const std::filesystem::path spritePath = spritesBaseDirectory / name;

        for (auto const& dir_entry : std::filesystem::directory_iterator{ spritePath / L"costumes" })
        {
            textures.push_back(IMG_LoadTexture(renderer, dir_entry.path().string().c_str()));
        }
    }

    inline SDL_Texture * getCostumeTexture() const {
        return this->textures[this->costumeNumber];
    }
};


#endif
