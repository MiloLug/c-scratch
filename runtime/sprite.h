#ifndef SPRITE_H
#define SPRITE_H

#include "config.h"

#include <filesystem>
#include <vector>
#include "math.h"
#include "include_sdl.h"
#include "pen/pen.h"


// The compiler can't inline it for some reason
#define __penDrawLine(x1, y1, x2, y2) \
    Pen::drawLine( \
        x1 + centerOffsetX, \
        y1 + centerOffsetY, \
        x2 + centerOffsetX, \
        y2 + centerOffsetY, \
        penSize, \
        penColor \
    )


const std::filesystem::path spritesBaseDirectory = L"sprites/";


struct SpriteDeclaration {
    const wchar_t * name;
    float x;
    float y;
    float width;
    float height;
    float direction;
    int costumeNumber;
    float size;
    int visible;
    int layerOrder;
};


class Sprite {
public:
    const wchar_t * name;
    float direction;
    int costumeNumber;
    float size;
    int visible;
    int layerOrder;
    float x;
    float y;
    float centerOffsetX;
    float centerOffsetY;

    SDL_FRect pos;
    std::vector<std::pair<SDL_Texture *, SDL_Surface *>> costumes = {};

    SDL_Surface * surfaceCache = NULL;  // Surface cache for the Pen's stamp
    bool shouldUpdateSurfaceCache = true;
    bool isPenDown = false;
    uint32_t penSize = 1;
    uint32_t penColor = 0xFFAF9F3F;


    Sprite(const SpriteDeclaration &decl):
        name{decl.name},
        direction{decl.direction - 90.0f},
        costumeNumber{decl.costumeNumber},
        size{decl.size},
        visible{decl.visible},
        layerOrder{decl.layerOrder},
        x{decl.x},
        y{decl.y},
        centerOffsetX{decl.width / 2.0f},
        centerOffsetY{decl.height / 2.0f},
        pos{
            .x{WINDOW_WIDTH / 2.0f - centerOffsetX + decl.x},
            .y{WINDOW_HEIGHT / 2.0f - centerOffsetY - decl.y},
            .w{decl.width},
            .h{decl.height},
        }
    {}

    inline void setX(float _x) {
        x = _x;
        _x = pos.x;
        pos.x = WINDOW_WIDTH / 2.0 - centerOffsetX + x;

        if (isPenDown)
            Pen_safe(__penDrawLine(_x, pos.y, pos.x, pos.y));
    }

    inline void setY(float _y) {
        y = _y;
        _y = pos.y;
        pos.y = WINDOW_HEIGHT / 2.0 - centerOffsetY - y;

        if (isPenDown)
            Pen_safe(__penDrawLine(pos.x, _y, pos.x, pos.y));
    }

    inline void goXY(float _x, float _y) {
        x = _x;
        _x = pos.x;
        pos.x = WINDOW_WIDTH / 2.0 - centerOffsetX + x;

        y = _y;
        _y = pos.y;
        pos.y = WINDOW_HEIGHT / 2.0 - centerOffsetY - y;

        if (isPenDown)
            Pen_safe(__penDrawLine(_x, _y, pos.x, pos.y));
    }

    inline void changeX(float offset) {
        pos.x += offset;
        x += offset;

        if (isPenDown)
            Pen_safe(__penDrawLine(pos.x - offset, pos.y, pos.x, pos.y));
    }

    inline void changeY(float offset) {
        pos.y -= offset;
        y -= offset;

        if (isPenDown)
            Pen_safe(__penDrawLine(pos.x, pos.y - offset, pos.x, pos.y));
    }

    inline void turnRight(float angle) {
        shouldUpdateSurfaceCache = true;

        direction += angle;
        direction = fmod(direction, 360.0);
    }

    inline void turnLeft(float angle) {
        shouldUpdateSurfaceCache = true;

        direction -= angle;
        direction = fmod(direction, 360.0);
    }

    inline void point(float angle) {
        shouldUpdateSurfaceCache = true;
        direction = fmod(angle - 90.0, 360.0);
    }

    inline void move(float distance) {
        float dX = distance * degCos(direction);
        float dY = distance * degSin(direction);

        pos.x += dX;
        pos.y += dY;
        x += dX;
        y += dY;

        if (isPenDown)
            Pen_safe(__penDrawLine(pos.x - dX, pos.y - dY, pos.x, pos.y));
    }

    inline float getDirection() const {
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

    inline SDL_Surface * getCostumeTransformedSurface() {
        if (!shouldUpdateSurfaceCache) return surfaceCache;
        
        if (surfaceCache != NULL)
            SDL_FreeSurface(surfaceCache);
        surfaceCache = rotozoomSurface(getCostumeSurface(), -direction, 1, 1);
        
        return surfaceCache;
    }


    inline void penStamp() {
        auto tmp = getCostumeTransformedSurface();

        Pen_safe(Pen::stamp(
            round((WINDOW_WIDTH - tmp->w) / 2.0 + x),
            round((WINDOW_HEIGHT - tmp->h) / 2.0 + y),
            tmp
        ));
    }

    inline void penSetColor(uint32_t color) {
        penColor = (color >> 24) == 0
            ? color | 0xFF000000
            : color;
    }

    inline void penSetSize(double size) {
        penSize = round(MAX_UNSAFE(size, 0));
    }

    inline void penDown() {
        isPenDown = true;
        Pen_safe(__penDrawLine(pos.x, pos.y, pos.x, pos.y));
    }

    inline void penUp() {
        isPenDown = false;
    }

    ~Sprite() {
        for(auto &costume : costumes) {
            SDL_DestroyTexture(costume.first);
            SDL_FreeSurface(costume.second);
        }
    }
};


#endif
