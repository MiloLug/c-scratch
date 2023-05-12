#ifndef SPRITE_BASE_H
#define SPRITE_BASE_H

#include "config.h"
#include "math/math.h"
#include "pen/pen.h"
#include "sdl.h"
#include "utils.h"
#include "value.h"
#include "coroutines.h"
#include "control_flow.h"

#include <filesystem>
#include <unordered_map>
#include <vector>


const std::filesystem::path ASSETS_BASE_DIR = L"assets/";


#define __penDrawLine(x1, y1, x2, y2)                                                              \
    Pen::drawLine(                                                                                 \
        x1 + pPointLT.x, y1 + pPointLT.y, x2 + pPointLT.x, y2 + pPointLT.y, penSize, penColor      \
    )

#ifdef NO_COORDINATES_LIMITATION
    #define __boundX(x) (x)
    #define __boundY(y) (y)
    #define __boundXUnsafe(x) (x)
    #define __boundYUnsafe(y) (y)
    #define __boundXMove(x, d) ((x) + (d))
    #define __boundYMove(y, d) ((y) + (d))
#else
    #define __boundX(x)                                                                            \
        ({                                                                                         \
            auto __x = (x);                                                                        \
            __x > MAX_X ? MAX_X : (__x < -MAX_X ? -MAX_X : __x);                                   \
        })
    #define __boundY(y)                                                                            \
        ({                                                                                         \
            auto __y = (y);                                                                        \
            __y > MAX_Y ? MAX_Y : (__y < -MAX_Y ? -MAX_Y : __y);                                   \
        })
    #define __boundXUnsafe(x) (x > MAX_X ? MAX_X : (x < -MAX_X ? -MAX_X : x))
    #define __boundYUnsafe(y) (y > MAX_Y ? MAX_Y : (y < -MAX_Y ? -MAX_Y : y))

    #define __boundXMove(x, d)                                                                     \
        ({                                                                                         \
            float __x2 = (x) + (d);                                                                \
            __x2 > MAX_X ? MAX_X : (__x2 < -MAX_X ? -MAX_X : __x2);                                \
        })

    #define __boundYMove(y, d)                                                                     \
        ({                                                                                         \
            float __y2 = (y) + (d);                                                                \
            __y2 > MAX_X ? MAX_X : (__y2 < -MAX_X ? -MAX_X : __y2);                                \
        })
#endif


class Movable {
protected:
    static constexpr float MAX_X = 30000.0f;
    static constexpr float MAX_Y = 30000.0f;

    float direction;
    float size;
    float x;
    float y;

    float sizeScaled;
    float wOrig;
    float hOrig;
    SDL_FPoint pPointOrig;
    SDL_FPoint pPoint;
    SDL_FPoint pPointLT;
    SDL_FPoint pRotationOffset;  // offset for the surface rotation
    SDL_FPoint pUnrotatedPoint;  // pivot with rotation offset cancellation
    float centerOffsetLTX;
    float centerOffsetLTY;
    float windowOffsetLTX;
    float windowOffsetLTY;

    SDL_FRect pos;

    bool shouldUpdateTransformCache = true;
    bool draggingEnabled = false;

    bool isPenDown = false;
    uint32_t penSize = 1;
    uint32_t penColor = 0xFFAF9F3F;


    force_inline__ void updateRotationOffset() {
        const auto tCos = degCos(direction);
        const auto tSin = degSin(direction);

        pRotationOffset.x = tCos * pPoint.x - tSin * pPoint.y - pPoint.x;
        pRotationOffset.y = tSin * pPoint.x + tCos * pPoint.y - pPoint.y;

        pUnrotatedPoint.x = pPoint.x - pRotationOffset.x;
        pUnrotatedPoint.y = pPoint.y - pRotationOffset.y;
    }

    /*
    * This function updates everything related to scaling and pivot position
    */
    force_inline__ void updateOffsetsAndPoints() {
        sizeScaled = size / 100.0f;
        pos.w = wOrig * sizeScaled;
        pos.h = hOrig * sizeScaled;
        pPoint.x = pPointOrig.x * sizeScaled;
        pPoint.y = pPointOrig.y * sizeScaled;
        pPointLT.x = pos.w / 2.0f + pPoint.x;
        pPointLT.y = pos.h / 2.0f - pPoint.y;
        windowOffsetLTX = WINDOW_CENTER_X - pPointLT.x;
        windowOffsetLTY = WINDOW_CENTER_Y - pPointLT.y;
        pos.x = windowOffsetLTX + x;
        pos.y = windowOffsetLTY - y;

        updateRotationOffset();

        shouldUpdateTransformCache = true;
    }

public:
    Movable(
        float _x, float _y, float pivotX, float pivotY, float w, float h, float dir, float _size
    ):
        direction{dir - 90.0f},
        size{_size},
        sizeScaled{_size / 100.0f},
        x{__boundXUnsafe(_x)},
        y{__boundYUnsafe(_y)},
        wOrig{w},
        hOrig{h},
        pPointOrig{
            .x{pivotX},
            .y{pivotY},
        },
        pPoint{
            .x{pivotX * sizeScaled},
            .y{pivotY * sizeScaled},
        },
        pPointLT{
            .x{(w * sizeScaled) / 2.0f + pPoint.x},
            .y{(h * sizeScaled) / 2.0f - pPoint.y},
        },
        windowOffsetLTX{WINDOW_CENTER_X - pPointLT.x},
        windowOffsetLTY{WINDOW_CENTER_Y - pPointLT.y},
        pos{
            .x{windowOffsetLTX + __boundXUnsafe(_x)},
            .y{windowOffsetLTY - __boundYUnsafe(_y)},
            .w{w * sizeScaled},
            .h{h * sizeScaled},
        } {
        updateRotationOffset();
    }

    /*
    * Coordinates & Movements
    */

    void setX(float _x) {
        x = __boundXUnsafe(_x);
        _x = pos.x;
        pos.x = windowOffsetLTX + x;

        if (isPenDown) Pen_safe(__penDrawLine(_x, pos.y, pos.x, pos.y));
    }

    void setY(float _y) {
        y = __boundYUnsafe(_y);
        _y = pos.y;
        pos.y = windowOffsetLTY - y;

        if (isPenDown) Pen_safe(__penDrawLine(pos.x, _y, pos.x, pos.y));
    }

    void changeX(float offset) {
        x = __boundX(x + offset);
        offset = pos.x;
        pos.x = windowOffsetLTX + x;

        if (isPenDown) Pen_safe(__penDrawLine(offset, pos.y, pos.x, pos.y));
    }

    void changeY(float offset) {
        y = __boundY(y + offset);
        offset = pos.y;
        pos.y = windowOffsetLTY - y;

        if (isPenDown) Pen_safe(__penDrawLine(pos.x, offset, pos.x, pos.y));
    }

    force_inline__ const float getX() { return x; }

    force_inline__ const float getY() { return y; }

    void setPivotXY(float x, float y) {
        pPointOrig.x = x;
        pPointOrig.y = y;
        updateOffsetsAndPoints();
    }

    force_inline__ const SDL_FPoint & getPivotLT() { return pPointLT; }

    void goXY(float _x, float _y) {
        x = __boundXUnsafe(_x);
        _x = pos.x;
        pos.x = windowOffsetLTX + x;

        y = __boundYUnsafe(_y);
        _y = pos.y;
        pos.y = windowOffsetLTY - y;

        if (isPenDown) Pen_safe(__penDrawLine(_x, _y, pos.x, pos.y));
    }

    void move(float distance) {
        x = __boundXMove(x, distance * (float)degCos(direction));
        y = __boundYMove(y, -distance * (float)degSin(direction));

        float oldX = pos.x;
        float oldY = pos.y;

        pos.x = windowOffsetLTX + x;
        pos.y = windowOffsetLTY - y;

        if (isPenDown) Pen_safe(__penDrawLine(oldX, oldY, pos.x, pos.y));
    }

    void goToPinter() { goXY(mouseState.x, mouseState.y); }

    void goToRandomPosition() {
        goXY(
            randInRange(-WINDOW_CENTER_X, WINDOW_CENTER_X),
            randInRange(-WINDOW_CENTER_Y, WINDOW_CENTER_Y)
        );
    }

    void goToSprite(Movable * sprite) { goXY(sprite->x, sprite->y); }

    Coroutine startDragging() {
        float xOffset = mouseState.x - x;
        float yOffset = mouseState.y - y;

        while(draggingEnabled && mouseState.isButtonDown) {
            goXY(mouseState.x - xOffset, mouseState.y - yOffset);
            cs_yield;
        }
        
        co_return;
    }

    force_inline__ bool isDraggingEnabled() {
        return draggingEnabled;
    }

    force_inline__ void enableDragging() {
        draggingEnabled = true;
    }

    force_inline__ void disableDragging() {
        draggingEnabled = false;
    }

    /*
    * Rotation
    */

    void turnRight(float angle) {
        shouldUpdateTransformCache = true;

        direction += angle;
        direction = fmod(direction, 360.0);

        updateRotationOffset();
    }

    void turnLeft(float angle) {
        shouldUpdateTransformCache = true;

        direction -= angle;
        direction = fmod(direction, 360.0);

        updateRotationOffset();
    }

    void point(float angle) {
        shouldUpdateTransformCache = true;
        direction = fmod(angle - 90.0, 360.0);

        updateRotationOffset();
    }

    void pointTowardsPointer() {
        shouldUpdateTransformCache = true;
        direction = (atan2(mouseState.x - x, mouseState.y - y) - M_PI_2) / M_RAD;

        updateRotationOffset();
    }

    void pointTowardsSprite(Movable * sprite) {
        shouldUpdateTransformCache = true;
        direction = (atan2(sprite->x - x, sprite->y - y) - M_PI_2) / M_RAD;

        updateRotationOffset();
    }

    force_inline__ float getDirection() const { return fmod(direction + 90.0, 360.0); }

    force_inline__ const float & getDirectionLT() { return direction; }

    /*
    * Sensing
    */

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

    bool isTouchingXY(float x1, float y1);

    force_inline__ bool isTouchingPointer() { return isTouchingXY(mouseState.x, mouseState.y); }

    /*
    * Dimensions
    */

    force_inline__ const SDL_FRect & getRectLT() { return pos; }

    void setDim(float w, float h) {
        wOrig = w;
        hOrig = h;
        updateOffsetsAndPoints();
    }

    void setSize(float _size) {
        if (_size != size) {
            size = _size;
            updateOffsetsAndPoints();
        }
    }

    force_inline__ const float & getSize() { return size; }

    void changeSize(float t) {
        if (t != 0) {
            size += t;
            updateOffsetsAndPoints();
        }
    }

    /*
    * Pen Tools
    */

    void penStamp();

    force_inline__ void penSetColor(uint32_t color) {
        penColor = color << 8 | ((color >> 24) == 0 ? 0xFF : color >> 24);
    }

    force_inline__ void penSetSize(double size) { penSize = round(MAX_UNSAFE(size, 0)); }

    void penDown() {
        isPenDown = true;
        Pen_safe(__penDrawLine(pos.x, pos.y, pos.x, pos.y));
    }

    force_inline__ void penUp() { isPenDown = false; }
};


class Costume {
public:
    const wchar_t * name;
    const wchar_t * fileName;
    float w;
    float h;
    float pX;
    float pY;

    SDL_Surface * surface = NULL;
    SDL_Texture * texture = NULL;

    Costume(
        const wchar_t * _name,
        const wchar_t * _fileName,
        float pivotX,
        float pivotY,
        float _w,
        float _h
    ):
        name{_name},
        fileName{_fileName},
        w{_w},
        h{_h},
        pX{pivotX},
        pY{pivotY} {}

    void init(SDL_Renderer * renderer, const std::filesystem::path & baseDir) {
        surface = IMG_Load((baseDir / fileName).string().c_str());
        if (!surface) {
            wprintf(L"Error: no costume file '%ls'\n", fileName);
        }
        if (surface->format->format != SDL_PIXELFORMAT_RGBA8888) {
            auto tmp = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
            SDL_FreeSurface(surface);
            surface = tmp;
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
    }

    ~Costume() {
        if (surface) {
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
    }
};


class SpriteBase {
protected:
    const std::filesystem::path spritePath;
    const std::filesystem::path costumesPath;
    uint64_t costumesNumber;
    uint64_t costumeIndex;
    Costume * currentCostume;
    std::vector<Costume> costumes;
    std::unordered_map<uint64_t, uint64_t> costumeIndexes;
    std::unordered_map<uint64_t, uint64_t>::iterator costumeIndexesEnd;

    force_inline__ void onCostumeSwitch() { currentCostume = &costumes[costumeIndex]; }

public:
    volatile bool __stopOtherScripts = false;

    SpriteBase(
        const std::filesystem::path & _spritePath,
        uint64_t _costumeIndex,
        const std::vector<Costume> & _costumes
    ):
        costumeIndex{_costumeIndex - 1},
        spritePath{_spritePath},
        costumesPath{_spritePath / L"costumes"},
        costumes{_costumes} {
        uint64_t i = 0;
        for (const auto & costume : _costumes) {
            costumeIndexes[fastHash(costume.name)] = i++;
        }
        costumeIndexesEnd = costumeIndexes.end();
        costumesNumber = _costumes.size();
        currentCostume = &costumes[costumeIndex];
    }

    void init(SDL_Renderer * renderer) {
        for (auto & costume : costumes) {
            costume.init(renderer, costumesPath);
#ifdef DEBUG
            wprintf(L"Sprite costumes loader: '%ls' OK\n", costume.name);
#endif
        }
    }

    /**
    * This method behaves just like scratch's sprite switching.
    * First it tries to switch using names (even if you pass a number).
    * Then it tries to use the number as a sprite index.
    */
    void switchCostumeTo(Value && value) {
        auto found = costumeIndexes.find(fastHash(value.toString()));
        if (found != costumeIndexesEnd) {
            costumeIndex = found->second;
        } else if (value.number > 0 && value.number <= costumesNumber) {
            costumeIndex = value.number - 1;
        }

        onCostumeSwitch();
    }
    void switchCostumeTo(String && str) {
        auto found = costumeIndexes.find(fastHash(str.data));
        if (found != costumeIndexesEnd) {
            costumeIndex = found->second;
        } else {
            auto num = String::strToNum(str, str.length);
            if (num > 0 && num <= costumesNumber) costumeIndex = num - 1;
        }

        onCostumeSwitch();
    }
    void switchCostumeTo(const wchar_t * str) {
        switchCostumeTo(String(wcslen(str), (wchar_t *)str, true, true));
    }

    void switchCostumeByIndex(uint64_t index) {
        if (index > 0 && index <= costumesNumber) costumeIndex = index - 1;
        onCostumeSwitch();
    }
    /**
    * For this method to work, you need to pass fastHash(L"some string"),
    * provided by the runtime/utils.h.
    * Also you can use just L"your string"_H
    */
    void switchCostumeByNameHash(uint64_t nameHash) {
        auto found = costumeIndexes.find(nameHash);
        if (found != costumeIndexesEnd) costumeIndex = found->second;
        onCostumeSwitch();
    }

    void nextCostume() { costumeIndex = (costumeIndex + 1) % costumesNumber; }

    SDL_Texture * getCostumeTexture() const { return currentCostume->texture; }

    SDL_Surface * getCostumeSurface() const { return currentCostume->surface; }

    double getCostumeIndex() { return costumeIndex + 1; }

    const wchar_t * getCostumeName() { return currentCostume->name; }

    void stopOtherScripts() { __stopOtherScripts = true; }

    SDL_Surface * getCostumeTransformedSurface() { return NULL; }
};


#endif
