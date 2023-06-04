#include "script_manager.h"

#include "actions.h"
#include "config.h"
#include "runtime/coroutines.h"
#include "runtime/sprite/sprite_base.h"
#include "runtime/debug.h"

#include <ctime>
#include <list>


ThreadSafeQueue<ScriptManager::CoroContainer> ScriptManager::newActiveCoros;
constinit ScriptManager::BindingsMap * ScriptManager::scriptBindingsStorage = nullptr;

// STATIC

/*
* Adds all scripts for the given `action` to the execution queue.
* Returns: number of scripts activated.
*/
uint32_t ScriptManager::triggerScripts(uint64_t action, Context * ctx) {
    uint32_t i = 0;
    auto actionBindings = scriptBindingsStorage->find(action);
    if (actionBindings != scriptBindingsStorage->end()) {
        for (auto & [sprite, coros] : actionBindings->second) {
            for (auto & coro : coros) {
                newActiveCoros.push({sprite, new Coroutine(coro(ctx))});
                i++;
            }
        }
    }

    return i;
}

/*
* Adds a coroutine the the list of "new coroutines".
* So eventually it will be managed and resumed by the scripts execution loop.
*/
void ScriptManager::manageCoroutine(SpriteBase * sprite, Coroutine * coroutine) {
    newActiveCoros.push({sprite, coroutine});
}

/*
* Merges `bindings` to the bindings storage, so you can dynamically add new scripts
* without losing old ones.
*/
void ScriptManager::bindScripts(const BindingsMap & bindings) {
    for (auto & [action, actionSprites] : bindings) {
        auto globalActionBindings = scriptBindingsStorage->find(action);
        if (globalActionBindings != scriptBindingsStorage->end()) {
            auto & globalActionSprites = (*globalActionBindings).second;

            for (auto & sprite : actionSprites) {
                globalActionSprites.push_back(sprite);
            }
        } else {
            (*scriptBindingsStorage)[action] = actionSprites;
        }
    }
}

void ScriptManager::bindScripts(SpriteBase * sprite, const SimpleBindingsMap & bindings) {
    for (auto & [action, actionScripts] : bindings) {
        auto globalActionBindings = scriptBindingsStorage->find(action);
        if (globalActionBindings != scriptBindingsStorage->end()) {
            auto & globalActionSprites = (*globalActionBindings).second;

            globalActionSprites.push_back({sprite, actionScripts});
        } else {
            (*scriptBindingsStorage)[action] = {{sprite, actionScripts}};
        }
    }
}

void ScriptManager::bindScripts(
    SpriteBase * sprite, uint64_t action, const CoroFunction & function
) {
    bindScripts(sprite, {{action, {function}}});
}

static void stopOtherScripts(
    SpriteBase * const & sprite,
    Coroutine * const & coro,
    std::list<ScriptManager::CoroContainer>::iterator & currentIter,
    std::list<ScriptManager::CoroContainer> & corosList,
    const std::list<ScriptManager::CoroContainer>::iterator & end
) {
    sprite->__stopOtherScripts = false;

    corosList.erase(currentIter);
    auto corosIter = corosList.begin();

    while (corosIter != end) {
        if (corosIter->first == sprite) {
            delete corosIter->second;
            corosList.erase(corosIter++);
        } else {
            corosIter++;
        }
    }
    corosList.push_back({sprite, coro});
}

/*
* This loop executes all scripts triggered by `triggerScripts`
*/
void ScriptManager::startScriptsLoop() {
    std::list<CoroContainer> activeCoros;
    CoroContainer newCoroutine;
    auto corosEnd = activeCoros.end();

#ifndef ENABLE_TURBO
    const int clocks_per_frame = CLOCKS_PER_SEC / NON_TURBO_CALCULATION_FPS;
    clock_t previous_time = clock();
#else
    #ifndef ENABLE_UNSAFE_NO_LOCKS
    int32_t unlockCounter = 0;
    Pen::pixels.take();
    #endif
#endif

    triggerScripts(ACTION_START);

    while (ScriptsShouldRun) {
#ifndef ENABLE_TURBO
        if (int(clock() - previous_time) < clocks_per_frame) continue;
        previous_time += clocks_per_frame;
#else
    #ifndef ENABLE_UNSAFE_NO_LOCKS
        if (++unlockCounter == TURBO_LOCK_WINDOW_CYCLES) {
            unlockCounter = 0;
            Pen::pixels.release();
            // Hope the graphics thread will have enough time to `.take()` the pixels in between.
            Pen::pixels.take();
        }
    #endif
#endif

        while (newActiveCoros.pop(newCoroutine)) {
            activeCoros.push_back(newCoroutine);
        }

        auto corosIter = activeCoros.begin();

        while (ScriptsShouldRun && corosIter != corosEnd) {
            auto & [sprite, coro] = *corosIter;

            if (!coro->done()) {
                coro->resume();

                if (!sprite || !sprite->__stopOtherScripts) {
                    corosIter++;
                } else {
                    stopOtherScripts(sprite, coro, corosIter, activeCoros, corosEnd);
                    break;
                }
            } else {
                delete coro;
                activeCoros.erase(corosIter++);
            }
        }
    }

#ifdef ENABLE_TURBO
    #ifndef ENABLE_UNSAFE_NO_LOCKS
    Pen::pixels.release();
    #endif
#endif
}

void ScriptManager::staticInit() {
    if (scriptBindingsStorage != nullptr) return;

    scriptBindingsStorage = new ScriptManager::BindingsMap;
}

// NON-STATIC

ScriptManager::ScriptManager() {
    isInitializer = true;
    staticInit();
}
ScriptManager::ScriptManager(const BindingsMap & bindings) {
    staticInit();
    bindScripts(bindings);
}
ScriptManager::ScriptManager(SpriteBase * sprite, const SimpleBindingsMap & bindings) {
    staticInit();
    bindScripts(sprite, bindings);
}
ScriptManager::ScriptManager(SpriteBase * sprite, uint64_t action, const CoroFunction & function) {
    staticInit();
    bindScripts(sprite, action, function);
}

ScriptManager::~ScriptManager() {
    if (isInitializer && scriptBindingsStorage != nullptr)
        delete scriptBindingsStorage;
    scriptBindingsStorage = nullptr;
}
