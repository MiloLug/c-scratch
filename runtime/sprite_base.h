#ifndef SPRITE_BASE_H
#define SPRITE_BASE_H

#include "config.h"
#include <filesystem>
#include <vector>
#include "math.h"
#include "sdl.h"
#include "pen/pen.h"


const std::filesystem::path ASSETS_BASE_DIR = L"assets/";


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

#define __boundX(x) ({auto __x = (x); __x > MAX_X ? MAX_X : (__x < -MAX_X ? -MAX_X : __x);})
#define __boundY(y) ({auto __y = (y); __y > MAX_Y ? MAX_Y : (__y < -MAX_Y ? -MAX_Y : __y);})
#define __boundXUnsafe(x) (x > MAX_X ? MAX_X : (x < -MAX_X ? -MAX_X : x))
#define __boundYUnsafe(y) (y > MAX_Y ? MAX_Y : (y < -MAX_Y ? -MAX_Y : y))

#define __boundXMove(x, d) ({  \
    float                      \
        __x2 = (x) + (d);      \
    __x2 > MAX_X ? MAX_X : (__x2 < -MAX_X ? -MAX_X : __x2); \
})

#define __boundYMove(y, d) ({  \
    float                      \
        __y2 = (y) + (d);      \
    __y2 > MAX_X ? MAX_X : (__y2 < -MAX_X ? -MAX_X : __y2); \
})


class Movable {
public:
    static constexpr float MAX_X = 30000.0f;
    static constexpr float MAX_Y = 30000.0f;

    float direction;
    float size;
    float x;
    float y;
    float centerOffsetX;
    float centerOffsetY;
    float windowCenterOffsetX;
    float windowCenterOffsetY;

    SDL_FRect pos;
    
    bool shouldUpdateSurfaceCache = true;
    bool isPenDown = false;
    uint32_t penSize = 1;
    uint32_t penColor = 0xFFAF9F3F;

    Movable(float _x, float _y, float w, float h, float dir, float _size):
        direction(direction - 90.0f),
        size{_size},
        x{__boundXUnsafe(_x)},
        y{__boundYUnsafe(_y)},
        centerOffsetX{w / 2.0f},
        centerOffsetY{h / 2.0f},
        windowCenterOffsetX{WINDOW_CENTER_X - centerOffsetX},
        windowCenterOffsetY{WINDOW_CENTER_Y - centerOffsetY},
        pos{
            .x{windowCenterOffsetX + __boundXUnsafe(_x)},
            .y{windowCenterOffsetY - __boundYUnsafe(_y)},
            .w{w},
            .h{h},
        }
    {}

    void setX(float _x) {
        x = __boundXUnsafe(_x);
        _x = pos.x;
        pos.x = windowCenterOffsetX + x;

        if (isPenDown)
            Pen_safe(__penDrawLine(_x, pos.y, pos.x, pos.y));
    }

    void setY(float _y) {
        y = __boundYUnsafe(_y);
        _y = pos.y;
        pos.y = windowCenterOffsetY - y;

        if (isPenDown)
            Pen_safe(__penDrawLine(pos.x, _y, pos.x, pos.y));
    }

    void goXY(float _x, float _y) {
        x = __boundXUnsafe(_x);
        _x = pos.x;
        pos.x = windowCenterOffsetX + x;

        y = __boundYUnsafe(_y);
        _y = pos.y;
        pos.y = windowCenterOffsetY - y;

        if (isPenDown)
            Pen_safe(__penDrawLine(_x, _y, pos.x, pos.y));
    }

    void goToPinter() {
        goXY(mouseState.x, mouseState.y);
    }

    void goToRandomPosition() {
        goXY(
            randInRange(-WINDOW_CENTER_X, WINDOW_CENTER_X),
            randInRange(-WINDOW_CENTER_Y, WINDOW_CENTER_Y)
        );
    }

    void goToSprite(Movable * sprite) {
        goXY(sprite->x, sprite->y);
    }

    void changeX(float offset) {
        x = __boundX(x + offset);
        offset = pos.x;
        pos.x = windowCenterOffsetX + x;

        if (isPenDown)
            Pen_safe(__penDrawLine(offset, pos.y, pos.x, pos.y));
    }

    void changeY(float offset) {
        y = __boundY(y + offset);
        offset = pos.y;
        pos.y = windowCenterOffsetY - y;

        if (isPenDown)
            Pen_safe(__penDrawLine(pos.x, offset, pos.x, pos.y));
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

    void pointTowardsPointer() {
        shouldUpdateSurfaceCache = true;
        direction = (atan2(mouseState.x - x, mouseState.y - y) - M_PI_2) / M_RAD;
    }

    void pointTowardsSprite(Movable * sprite) {
        shouldUpdateSurfaceCache = true;
        direction = (atan2(sprite->x - x, sprite->y - y) - M_PI_2) / M_RAD;
    }

    void move(float distance) {
        x = __boundXMove(x, distance * (float)degCos(direction));
        y = __boundYMove(y, -distance * (float)degSin(direction));

        float oldX = pos.x;
        float oldY = pos.y;

        pos.x = windowCenterOffsetX + x;
        pos.y = windowCenterOffsetY - y;

        if (isPenDown)
            Pen_safe(__penDrawLine(oldX, oldY, pos.x, pos.y));
    }

    float getDirection() const {
        return fmod(direction + 90.0, 360.0);
    }

    float getPointerDistance() {
        const auto xDiff = mouseState.x - pos.x;
        const auto yDiff = mouseState.y - pos.y;
        return sqrt(xDiff * xDiff + yDiff * yDiff);
    }

    float getSpriteDistance(Movable * sprite) {
        const auto xDiff = sprite->x - pos.x;
        const auto yDiff = sprite->y - pos.y;
        return sqrt(xDiff * xDiff + yDiff * yDiff);
    }

    bool isTouchingPointer();

    void penStamp();

    void penSetColor(uint32_t color) {
        penColor = color << 8 | (
            (color >> 24) == 0 ? 0xFF : color >> 24
        );
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
};


class SpriteBase {
public:
    const std::filesystem::path spritePath;
    uint64_t costumeNumber;
    
    std::vector<std::pair<SDL_Texture *, SDL_Surface *>> costumes = {};
    
    volatile bool stopScripts = false;

    SpriteBase(const std::filesystem::path _spritePath, uint64_t costumeNum):
        costumeNumber{costumeNum},
        spritePath{_spritePath}
    {}

    void init(SDL_Renderer * renderer) {
        for (auto const& dir_entry : std::filesystem::directory_iterator{ spritePath / L"costumes" }) {
            auto surface = IMG_Load(dir_entry.path().string().c_str());
            if (surface->format->format != SDL_PIXELFORMAT_ARGB8888) {
                auto tmp = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);
                SDL_FreeSurface(surface);
                surface = tmp;
            }
            costumes.push_back({SDL_CreateTextureFromSurface(renderer, surface), surface});

            #ifdef DEBUG
                wprintf(L"Sprite costumes loader: '%s' OK\n", dir_entry.path().string().c_str());
            #endif
        }
    }

    SDL_Texture * getCostumeTexture() const {
        return this->costumes[this->costumeNumber].first;
    }

    SDL_Surface * getCostumeSurface() const {
        return this->costumes[this->costumeNumber].second;
    }

    SDL_Surface * getCostumeTransformedSurface() {
        return NULL;
    }

    virtual ~SpriteBase() {
        for(auto &costume : costumes) {
            SDL_DestroyTexture(costume.first);
            SDL_FreeSurface(costume.second);
        }
    }
};


#endif
