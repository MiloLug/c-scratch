#ifndef SPRITE_H
#define SPRITE_H

#include <filesystem>
#include <vector>
#include <thread>
#include <cmath>
#include "include_sdl.h"
#include "../config.h"
#include <cwchar>
#include <utility>


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
    std::vector<std::pair<SDL_Texture*, SDL_Surface*>> costumes = {};

    inline void setX(float _x) {
        pos.x = (WINDOW_WIDTH - this->pos.w) / 2.0 + _x;
        x = _x;
    }

    inline void setY(float _y) {
        pos.y = (WINDOW_HEIGHT - this->pos.h) / 2.0 - _y;
        y = _y;
    }

    inline void goXY(float _x, float _y) {
        pos.x = (WINDOW_WIDTH - pos.w) / 2.0 + _x;
        pos.y = (WINDOW_HEIGHT - pos.h) / 2.0 - _y;
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

        for (auto const& dir_entry : std::filesystem::directory_iterator{ spritePath / L"costumes" }) {
            auto surface = IMG_Load(dir_entry.path().string().c_str());
            if (surface->format->format != SDL_PIXELFORMAT_ARGB8888) {
                auto tmp = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);
                SDL_FreeSurface(surface);
                surface = tmp;
                for(int i = 0; i < tmp->clip_rect.w * tmp->clip_rect.h; i++) {
                    ((uint32_t *)tmp->pixels)[i] = 0xFF00FF00;
                }
            }
            costumes.push_back({SDL_CreateTextureFromSurface(renderer, surface), surface});
        }
    }

    inline SDL_Texture * getCostumeTexture() const {
        return this->costumes[this->costumeNumber].first;
    }

    inline SDL_Surface * getCostumeSurface() const {
        return this->costumes[this->costumeNumber].second;
    }

    ~Sprite() {
        for(auto &costume : costumes) {
            SDL_DestroyTexture(costume.first);
            SDL_FreeSurface(costume.second);
        }
    }
};


#endif
