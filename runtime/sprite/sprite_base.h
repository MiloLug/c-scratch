#ifndef SPRITE_BASE_H
#define SPRITE_BASE_H

#include "runtime/control_flow.h"
#include "runtime/coroutines.h"
#include "runtime/math/math.h"
#include "runtime/pen/pen.h"
#include "runtime/sdl.h"
#include "runtime/utils.h"
#include "runtime/value.h"
#include "sprite_manager.h"

#include <filesystem>
#include <unordered_map>
#include <vector>


const std::filesystem::path ASSETS_BASE_DIR = L"assets/";


#define __penDrawLine(x1, y1, x2, y2)                                                              \
    Pen::drawLine(                                                                                 \
        x1 + pPointLT.x, y1 + pPointLT.y, x2 + pPointLT.x, y2 + pPointLT.y, penSize, penColor      \
    )

static constexpr auto __MAX_X = 30000.0f;
static constexpr auto __MAX_Y = 30000.0f;

#ifdef NO_COORDINATES_LIMITATION
    #define __boundX(x) (x)
    #define __boundY(y) (y)
    #define __boundXMove(x, d) ((x) + (d))
    #define __boundYMove(y, d) ((y) + (d))
#elif (defined(COMPILER_CLANG) || defined(COMPILER_GNU)) && !defined(COMPILER_ENV_VS)
    #define __boundX(x)                                                                            \
        ({                                                                                         \
            auto __x = (x);                                                                        \
            __x > __MAX_X ? __MAX_X : (__x < -__MAX_X ? -__MAX_X : __x);                           \
        })
    #define __boundY(y)                                                                            \
        ({                                                                                         \
            auto __y = (y);                                                                        \
            __y > __MAX_Y ? __MAX_Y : (__y < -__MAX_Y ? -__MAX_Y : __y);                           \
        })

    #define __boundXMove(x, d)                                                                     \
        ({                                                                                         \
            auto __x2 = (x) + (d);                                                                 \
            __x2 > __MAX_X ? __MAX_X : (__x2 < -__MAX_X ? -__MAX_X : __x2);                        \
        })

    #define __boundYMove(y, d)                                                                     \
        ({                                                                                         \
            auto __y2 = (y) + (d);                                                                 \
            __y2 > __MAX_Y ? __MAX_Y : (__y2 < -__MAX_Y ? -__MAX_Y : __y2);                        \
        })
#else
static force_inline__ auto __boundX(auto x) {
    return x > __MAX_X ? __MAX_X : (x < -__MAX_X ? -__MAX_X : x);
}
static force_inline__ auto __boundY(auto y) {
    return y > __MAX_Y ? __MAX_Y : (y < -__MAX_Y ? -__MAX_Y : y);
}
static force_inline__ auto __boundXMove(auto x, auto d) {
    auto x2 = x + d;
    return x2 > __MAX_X ? __MAX_X : (x2 < -__MAX_X ? -__MAX_X : x2);
}
static force_inline__ auto __boundYMove(auto y, auto d) {
    auto y2 = y + d;
    return y2 > __MAX_Y ? __MAX_Y : (y2 < -__MAX_Y ? -__MAX_Y : y2);
}
#endif


class Movable {
protected:
    float direction;
    float size;  // size in %
    float x;  // 0-center bases coordinates
    float y;

    // LT means Left-Top coordinate system or left-top corner

    float sizeMultiplier;  // basically (size / 100)
    float wOrig;  // original dimensions (before scaling etc)
    float hOrig;
    SDL_FPoint pPointOrig;  // original pivot point
    SDL_FPoint pPoint;  // pivot point after scaling
    SDL_FPoint pPointLT;  // pivot relative to sprite's left-top corner (for renderer)

    /*
    * Rotating and object around some pivot, we rotate it around its real center and then offset
    *   pRotationOffset represents this offset after rotation.
    * pUnrotatedPoint represents the pivot with rotation offset applied. It's useful
    *   for calculating coordinates inside a rotated sprite related to the global coordinates
    */
    SDL_FPoint pRotationOffset;
    SDL_FPoint pUnrotatedPoint;

    /*
    * Offset of the sprite's LT corner relative to the window's corner for x=y=0
    * It simplifies sprite LT coordinates calculations in many places
    */
    float windowOffsetLTX;
    float windowOffsetLTY;

    SDL_FRect pos;  // sprites scaled rect (for renderer)

    double transformCacheVersion = DBL_MIN + 1;
    bool draggingEnabled = false;

    bool isPenDown = false;
    float penSize = 1;
    uint32_t penColor = 0xFFAF9F3F;  // RGBA color


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
        sizeMultiplier = size / 100.0f;
        pos.w = wOrig * sizeMultiplier;
        pos.h = hOrig * sizeMultiplier;
        pPoint.x = pPointOrig.x * sizeMultiplier;
        pPoint.y = pPointOrig.y * sizeMultiplier;
        pPointLT.x = pos.w / 2.0f + pPoint.x;
        pPointLT.y = pos.h / 2.0f - pPoint.y;
        windowOffsetLTX = WINDOW_CENTER_X - pPointLT.x;
        windowOffsetLTY = WINDOW_CENTER_Y - pPointLT.y;
        pos.x = windowOffsetLTX + x;
        pos.y = windowOffsetLTY - y;

        updateRotationOffset();
    }

public:
    Movable(
        float _x, float _y, float pivotX, float pivotY, float w, float h, float dir, float _size
    ):
        direction{dir - 90.0f},
        size{_size},
        sizeMultiplier{_size / 100.0f},
        x{__boundX(_x)},
        y{__boundY(_y)},
        wOrig{w},
        hOrig{h},
        pPointOrig{
            .x = pivotX,
            .y = pivotY,
        },
        pPoint{
            .x = pivotX * sizeMultiplier,
            .y = pivotY * sizeMultiplier,
        },
        pPointLT{
            .x = (w * sizeMultiplier) / 2.0f + pPoint.x,
            .y = (h * sizeMultiplier) / 2.0f - pPoint.y,
        },
        windowOffsetLTX{WINDOW_CENTER_X - pPointLT.x},
        windowOffsetLTY{WINDOW_CENTER_Y - pPointLT.y},
        pos{
            .x = windowOffsetLTX + __boundX(_x),
            .y = windowOffsetLTY - __boundY(_y),
            .w = w * sizeMultiplier,
            .h = h * sizeMultiplier,
        } {
        updateRotationOffset();
    }

    /*
    * Coordinates & Movements
    */

    void setX(float _x) {
        x = __boundX(_x);
        _x = pos.x;
        pos.x = windowOffsetLTX + x;

        if (isPenDown) Pen_safe(__penDrawLine(_x, pos.y, pos.x, pos.y));
    }

    void setY(float _y) {
        y = __boundY(_y);
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
        transformCacheVersion++;
    }

    force_inline__ const SDL_FPoint & getPivotLT() { return pPointLT; }

    void goXY(float _x, float _y) {
        x = __boundX(_x);
        _x = pos.x;
        pos.x = windowOffsetLTX + x;

        y = __boundY(_y);
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

    void goToSprite(OneOfT<const wchar_t *, uint64_t> auto spriteName) {
        auto sprite = (Movable *)SpriteManager::getByName(spriteName);
        if (sprite && sprite != this) goToSprite(sprite);
    }

    force_inline__ void goToSprite(Arg spriteName) {
        goToSprite(spriteName.toString());
    }

    Coroutine startDragging() {
        float xOffset = mouseState.x - x;
        float yOffset = mouseState.y - y;

        while (draggingEnabled && mouseState.isButtonDown) {
            goXY(mouseState.x - xOffset, mouseState.y - yOffset);
            cs_pass;
        }

        cs_stop;
    }

    /// @param time in seconds 
    Coroutine glideXY(double time, float _x, float _y) {
        auto dX = _x - x, dY = _y - y;
        _x = x;
        _y = y;

        time = csTime.sToNS(time);
        double startTime = csTime.currentTime, endTime = csTime.currentTime + time, currentTime = 0,
               step = 0;

        while ((currentTime = csTime.currentTime) < endTime) {
            step = (currentTime - startTime) / time;
            goXY(_x + dX * step, _y + dY * step);
            cs_pass;
        }

        goXY(_x + dX, _y + dY);  // just in case
        cs_stop;
    }

    /// @param time in seconds 
    force_inline__ Coroutine glideToRandomPosition(double time) {
        return glideXY(
            time,
            randInRange(-WINDOW_CENTER_X, WINDOW_CENTER_X),
            randInRange(-WINDOW_CENTER_Y, WINDOW_CENTER_Y)
        );
    }

    /// @param time in seconds 
    force_inline__ Coroutine glideToPointer(double time) {
        return glideXY(time, mouseState.x, mouseState.y);
    }

    /// @param time in seconds 
    force_inline__ Coroutine glideToSprite(double time, Movable * sprite) {
        return glideXY(time, sprite->x, sprite->y);
    }

    /// @param time in seconds 
    /// @param spriteName you can use either sprite name or it's hash: (L"my sprite"_H)
    Coroutine glideToSprite(double time, OneOfT<const wchar_t *, uint64_t> auto spriteName) {
        Movable * sprite = (Movable *)SpriteManager::getByName(spriteName);
        if (sprite && sprite != this) cs_wait glideXY(time, sprite->x, sprite->y);
        cs_stop;
    }

    /// @param time in seconds 
    Coroutine glideToSprite(double time, Arg spriteName) {
        return glideToSprite(time, spriteName.toString());
    }

    force_inline__ bool isDraggingEnabled() { return draggingEnabled; }

    force_inline__ void enableDragging() { draggingEnabled = true; }

    force_inline__ void disableDragging() { draggingEnabled = false; }

    /*
    * Rotation
    */

    void turnRight(float angle) {
        direction += angle;
        direction = fmod(direction, 360.0);

        updateRotationOffset();
        transformCacheVersion++;
    }

    void turnLeft(float angle) {
        direction -= angle;
        direction = fmod(direction, 360.0);

        updateRotationOffset();
        transformCacheVersion++;
    }

    void point(float angle) {
        direction = fmod(angle - 90.0, 360.0);

        updateRotationOffset();
        transformCacheVersion++;
    }

    void pointTowardsPointer() {
        direction = (atan2(mouseState.x - x, mouseState.y - y) - M_PI_2) / M_RAD;

        updateRotationOffset();
        transformCacheVersion++;
    }

    void pointTowardsSprite(Movable * sprite) {
        direction = (atan2(sprite->x - x, sprite->y - y) - M_PI_2) / M_RAD;

        updateRotationOffset();
        transformCacheVersion++;
    }

    /// @param spriteName you can use either sprite name or it's hash: (L"my sprite"_H)
    void pointTowardsSprite(OneOfT<const wchar_t *, uint64_t> auto spriteName) {
        Movable * sprite = (Movable *)SpriteManager::getByName(spriteName);
        if (sprite && sprite != this) pointTowardsSprite(sprite);
    }

    force_inline__ void pointTowardsSprite(Arg spriteName) {
        pointTowardsSprite(spriteName.toString());
    }

    force_inline__ float getDirection() const { return fmod(direction + 90.0, 360.0); }

    force_inline__ const float & getDirectionLT() const { return direction; }

    /*
    * Sensing
    */

    float getPointerDistance() const {
        const auto xDiff = mouseState.x - pos.x;
        const auto yDiff = mouseState.y - pos.y;
        return sqrt(xDiff * xDiff + yDiff * yDiff);
    }

    float getSpriteDistance(Movable * sprite) const {
        const auto xDiff = sprite->x - pos.x;
        const auto yDiff = sprite->y - pos.y;
        return sqrt(xDiff * xDiff + yDiff * yDiff);
    }

    /// @param spriteName you can use either sprite name or it's hash: (L"my sprite"_H)
    float getSpriteDistance(OneOfT<const wchar_t *, uint64_t> auto spriteName) const {
        auto sprite = (Movable *)SpriteManager::getByName(spriteName);
        if (sprite && sprite != this) return getSpriteDistance(sprite);
        return 0;
    }

    float getSpriteDistance(Arg spriteName) const {
        return getSpriteDistance(spriteName.toString());
    }

    bool isTouchingXY(float x1, float y1) const;

    force_inline__ bool isTouchingPointer() const { return isTouchingXY(mouseState.x, mouseState.y); }

    /*
    * Dimensions
    */

    force_inline__ const SDL_FRect & getRectLT() const { return pos; }

    void setDim(float w, float h) {
        wOrig = w;
        hOrig = h;
        updateOffsetsAndPoints();
        transformCacheVersion++;
    }

    void setSize(float _size) {
        if (_size != size) {
            size = _size;
            updateOffsetsAndPoints();
            transformCacheVersion++;
        }
    }

    force_inline__ const float & getSize() { return size; }

    void changeSize(float t) {
        if (t != 0) {
            size += t;
            updateOffsetsAndPoints();
            transformCacheVersion++;
        }
    }

    /*
    * Pen Tools
    */

    void penStamp();

    /// @param color ARGB 32 bit color
    force_inline__ void penSetColor(uint32_t color) {
        penColor = color << 8 | ((color >> 24) == 0 ? 0xFF : color >> 24);
    }

    force_inline__ void penSetSize(double size) { penSize = MAX_UNSAFE(size, 0.0); }

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

    SDL_Surface * surface = nullptr;
    SDL_Texture * texture = nullptr;
    SDL_Surface * surfaceCache = nullptr;
    double transformCacheVersion = DBL_MIN;

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
        if (surfaceCache) SDL_FreeSurface(surfaceCache);
    }
};


class SpriteBase {
protected:
    uint64_t costumesNumber;
    uint64_t costumeIndex;
    Costume * currentCostume;
    std::vector<Costume> costumes;
    std::unordered_map<uint64_t, uint64_t> costumeIndexes;

    force_inline__ void onCostumeSwitch() { currentCostume = &costumes[costumeIndex]; }

public:
    const std::filesystem::path spritePath;
    const std::filesystem::path costumesPath;
    const std::filesystem::path dataPath;

    volatile bool __stopOtherScripts = false;


    SpriteBase(
        const std::filesystem::path & _spritePath,
        uint64_t _costumeIndex,
        const std::vector<Costume> & _costumes
    ):
        costumeIndex{_costumeIndex - 1},
        spritePath{_spritePath},
        costumesPath{_spritePath / L"costumes"},
        dataPath{_spritePath / L"data"},
        costumes{_costumes} {
        uint64_t i = 0;
        for (const auto & costume : _costumes) {
            costumeIndexes[fastHash(costume.name)] = i++;
        }
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
    void switchCostumeTo(Arg value) {
        costumeIndex = findOr(
            costumeIndexes,
            fastHash(value.toString()),
            value.number > 0 && value.number <= costumesNumber ? value.number - 1 : costumeIndex
        );

        onCostumeSwitch();
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
        costumeIndex = findOr(costumeIndexes, nameHash, costumeIndex);
        onCostumeSwitch();
    }

    void nextCostume() {
        costumeIndex = (costumeIndex + 1) % costumesNumber;
        onCostumeSwitch();
    }

    SDL_Texture * getCostumeTexture() const { return currentCostume->texture; }

    SDL_Surface * getCostumeSurface() const { return currentCostume->surface; }

    double getCostumeIndex() { return costumeIndex + 1; }

    const wchar_t * getCostumeName() { return currentCostume->name; }

    void stopOtherScripts() { __stopOtherScripts = true; }

    SDL_Surface * getCostumeTransformedSurface() { return NULL; }
};


#endif
