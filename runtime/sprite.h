#ifndef SPRITE_H
#define SPRITE_H

#include "config.h"

#include <filesystem>
#include <vector>
#include "math.h"
#include "sdl.h"
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
constexpr float SPRITE_MAX_X = 30000.0f;
constexpr float SPRITE_MAX_Y = 30000.0f;


#define __boundX(x) ({auto __x = (x); __x > SPRITE_MAX_X ? SPRITE_MAX_X : (__x < -SPRITE_MAX_X ? -SPRITE_MAX_X : __x);})
#define __boundY(y) ({auto __y = (y); __y > SPRITE_MAX_Y ? SPRITE_MAX_Y : (__y < -SPRITE_MAX_Y ? -SPRITE_MAX_Y : __y);})
#define __boundXUnsafe(x) (x > SPRITE_MAX_X ? SPRITE_MAX_X : (x < -SPRITE_MAX_X ? -SPRITE_MAX_X : x))
#define __boundYUnsafe(y) (y > SPRITE_MAX_Y ? SPRITE_MAX_Y : (y < -SPRITE_MAX_Y ? -SPRITE_MAX_Y : y))

#define __boundXMove(x, d) ({  \
    float                      \
        __x1 = (x),            \
        __d = (d),             \
        __x2 = __x1 + __d;     \
    __x2 > SPRITE_MAX_X ? SPRITE_MAX_X : (__x2 < -SPRITE_MAX_X ? -SPRITE_MAX_X : __x2); \
})

#define __boundYMove(y, d) ({  \
    float                      \
        __y1 = (y),            \
        __d = (d),             \
        __y2 = __y1 + __d;     \
    __y2 > SPRITE_MAX_X ? SPRITE_MAX_X : (__y2 < -SPRITE_MAX_X ? -SPRITE_MAX_X : __y2); \
})


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
    float windowCenterOffsetX;
    float windowCenterOffsetY;

    SDL_FRect pos;
    std::vector<std::pair<SDL_Texture *, SDL_Surface *>> costumes = {};
    
    volatile bool stopScripts = false;

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
        x{__boundXUnsafe(decl.x)},
        y{__boundYUnsafe(decl.y)},
        centerOffsetX{decl.width / 2.0f},
        centerOffsetY{decl.height / 2.0f},
        windowCenterOffsetX{WINDOW_CENTER_X - centerOffsetX},
        windowCenterOffsetY{WINDOW_CENTER_Y - centerOffsetY},
        pos{
            .x{windowCenterOffsetX + __boundXUnsafe(decl.x)},
            .y{windowCenterOffsetY - __boundYUnsafe(decl.y)},
            .w{decl.width},
            .h{decl.height},
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

    void changeX(float offset) {
        x = __boundX(x + offset);
        pos.x = windowCenterOffsetX + x;

        if (isPenDown)
            Pen_safe(__penDrawLine(pos.x - offset, pos.y, pos.x, pos.y));
    }

    void changeY(float offset) {
        y = __boundY(y + offset);
        pos.y = windowCenterOffsetY - y;

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
        x = __boundXMove(x, distance * (float)degCos(direction));
        y = __boundYMove(y, distance * (float)degSin(direction));

        float oldX = pos.x;
        float oldY = pos.y;

        pos.x = windowCenterOffsetX + x;
        pos.y = windowCenterOffsetX - y;

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

    bool isTouchingPointer() {
        const auto tmpSurface = getCostumeTransformedSurface();

        const auto
            cOffsetX = tmpSurface->w / 2,
            cOffsetY = tmpSurface->h / 2;

        if (
            mouseState.x > (x + cOffsetX)
            || mouseState.x < (x - cOffsetX)
            || mouseState.y > (y + cOffsetY)
            || mouseState.y < (y - cOffsetY)
        )
            return false;
        
        const uint64_t
            pixelX = round((float)mouseState.x - x + cOffsetX),
            pixelY = round(y - mouseState.y + cOffsetY);
        
        return (((uint32_t *)tmpSurface->pixels)[tmpSurface->w * pixelY + pixelX] >> 24) != 0;
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
            round(WINDOW_CENTER_X - (float)tmp->w / 2.0 + x),
            round(WINDOW_CENTER_Y - (float)tmp->h / 2.0 - y),
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

    void stopSpriteScripts() {
        stopScripts = true;
    }

    ~Sprite() {
        for(auto &costume : costumes) {
            SDL_DestroyTexture(costume.first);
            SDL_FreeSurface(costume.second);
        }
    }
};


#endif
