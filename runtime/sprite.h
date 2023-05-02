#ifndef SPRITE_H
#define SPRITE_H

#include "config.h"

#include <filesystem>
#include <vector>
#include "math.h"
#include "include_sdl.h"
#include "pen/pen.h"


// The compiler can't inline it for some reason, when used as a function
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
constexpr float WINDOW_CENTER_X = WINDOW_WIDTH / 2.0f;
constexpr float WINDOW_CENTER_Y = WINDOW_HEIGHT / 2.0f;


struct SpriteDeclaration {
    const wchar_t * name;
    float x;
    float y;
    float width;
    float height;
    float direction;
    uint64_t costumeNumber;
    float size;
    bool visible;
    uint64_t layerOrder;
};


class Sprite {
public:
    const wchar_t * name;
    float direction;
    uint64_t costumeNumber;
    float size;
    bool visible;
    uint64_t layerOrder;
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
        costumeNumber{decl.costumeNumber - 1},
        size{decl.size},
        visible{decl.visible},
        layerOrder{decl.layerOrder},
        x{decl.x},
        y{decl.y},
        centerOffsetX{decl.width / 2.0f},
        centerOffsetY{decl.height / 2.0f},
        pos{
            .x{WINDOW_CENTER_X - centerOffsetX + decl.x},
            .y{WINDOW_CENTER_Y - centerOffsetY - decl.y},
            .w{decl.width},
            .h{decl.height},
        }
    {}

    void setX(float _x) {
        x = _x;
        _x = pos.x;
        pos.x = WINDOW_CENTER_X - centerOffsetX + x;

        if (isPenDown)
            Pen_safe(__penDrawLine(_x, pos.y, pos.x, pos.y));
    }

    void setY(float _y) {
        y = _y;
        _y = pos.y;
        pos.y = WINDOW_CENTER_Y - centerOffsetY - y;

        if (isPenDown)
            Pen_safe(__penDrawLine(pos.x, _y, pos.x, pos.y));
    }

    void goXY(float _x, float _y) {
        x = _x;
        _x = pos.x;
        pos.x = WINDOW_CENTER_X - centerOffsetX + x;

        y = _y;
        _y = pos.y;
        pos.y = WINDOW_CENTER_Y - centerOffsetY - y;

        if (isPenDown)
            Pen_safe(__penDrawLine(_x, _y, pos.x, pos.y));
    }

    void changeX(float offset) {
        pos.x += offset;
        x += offset;

        if (isPenDown)
            Pen_safe(__penDrawLine(pos.x - offset, pos.y, pos.x, pos.y));
    }

    void changeY(float offset) {
        pos.y -= offset;
        y -= offset;

        if (isPenDown)
            Pen_safe(__penDrawLine(pos.x, pos.y - offset, pos.x, pos.y));
    }

    void turnRight(float angle) {
        shouldUpdateSurfaceCache = true;

        direction += angle;
        direction = fmod(direction, 360.0);
    }

    void turnLeft(float angle) {
        shouldUpdateSurfaceCache = true;

        direction -= angle;
        direction = fmod(direction, 360.0);
    }

    void point(float angle) {
        shouldUpdateSurfaceCache = true;
        direction = fmod(angle - 90.0, 360.0);
    }

    void move(float distance) {
        float dX = distance * degCos(direction);
        float dY = distance * degSin(direction);

        pos.x += dX;
        pos.y += dY;
        x += dX;
        y += dY;

        if (isPenDown)
            Pen_safe(__penDrawLine(pos.x - dX, pos.y - dY, pos.x, pos.y));
    }

    float getDirection() const {
        return fmod(direction + 90.0, 360.0);
    }


    void init(SDL_Renderer * renderer) {
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

    SDL_Texture * getCostumeTexture() const {
        return this->costumes[this->costumeNumber].first;
    }

    SDL_Surface * getCostumeSurface() const {
        return this->costumes[this->costumeNumber].second;
    }

    SDL_Surface * getCostumeTransformedSurface() {
        if (!shouldUpdateSurfaceCache) return surfaceCache;
        
        if (surfaceCache != NULL)
            SDL_FreeSurface(surfaceCache);
        surfaceCache = rotozoomSurface(getCostumeSurface(), -direction, 1, 1);
        
        return surfaceCache;
    }


    void penStamp() {
        auto tmp = getCostumeTransformedSurface();

        Pen_safe(Pen::stamp(
            round(WINDOW_CENTER_X - tmp->w / 2.0 + x),
            round(WINDOW_CENTER_Y - tmp->h / 2.0 + y),
            tmp
        ));
    }

    void penSetColor(uint32_t color) {
        penColor = (color >> 24) == 0
            ? color | 0xFF000000
            : color;
    }

    void penSetSize(double size) {
        penSize = round(MAX_UNSAFE(size, 0));
    }

    void penDown() {
        isPenDown = true;
        Pen_safe(__penDrawLine(pos.x, pos.y, pos.x, pos.y));
    }

    void penUp() {
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
