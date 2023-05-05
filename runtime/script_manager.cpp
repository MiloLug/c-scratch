#include "script_manager.h"
#include <list>
#include <ctime>
#include "coroutines.h"


volatile bool ScriptManager::shouldRun = true;
ThreadSafeQueue<ScriptManager::CoroContainer> ScriptManager::newActiveCoros;
constinit std::unique_ptr<ScriptManager::BindingsMap> ScriptManager::scriptBindingsStorage = nullptr;

// STATIC

/*
* Adds all scripts for the given `action` to the execution queue.
*/
void ScriptManager::triggerScripts(uint32_t action) {
    auto actionBindings = scriptBindingsStorage->find(action);
    if (actionBindings != scriptBindingsStorage->end()) {
        for (auto &[sprite, coros] : actionBindings->second) {
            for (auto &coro : coros) {
                newActiveCoros.push({sprite, new Coroutine(coro())});
            }
        }
    }
}

/*
* Merges `bindings` to the bindings storage, so you can dynamically add new scripts
* without losing old ones.
*/
void ScriptManager::bindScripts(const BindingsMap &bindings) {
    for (auto &[action, actionSprites] : bindings) {

        auto globalActionBindings = scriptBindingsStorage->find(action);
        if (globalActionBindings != scriptBindingsStorage->end()) {
            auto &globalActionSprites = (*globalActionBindings).second;

            for (auto &sprite : actionSprites) {
                globalActionSprites.push_back(sprite);
            }
        } else {
            (*scriptBindingsStorage)[action] = actionSprites;
        }
    }
}

/*
* This loop executes all scripts triggered by `triggerScripts`
*/
void ScriptManager::startScriptsLoop() {
    std::list<CoroContainer> activeCoros;
    CoroContainer newCoroutine;

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

    while (shouldRun) {
        #ifndef ENABLE_TURBO
            if (int(clock() - previous_time) < clocks_per_frame)
                continue;
            previous_time += clocks_per_frame;
        #else
            #ifndef ENABLE_UNSAFE_NO_LOCKS
                unlockCounter++;
                if (unlockCounter == TURBO_LOCK_WINDOW_CYCLES) {
                    unlockCounter = 0;
                    Pen::pixels.release();
                    // Hope the graphics thread will have enough time to `.take()` the pixels in between.
                    Pen::pixels.take();
                }
            #endif
        #endif
        
        while(newActiveCoros.pop(newCoroutine)) {
            activeCoros.push_back(newCoroutine);
        }

        auto corosIter = activeCoros.begin();
        auto corosEnd = activeCoros.end();

        while(corosIter != corosEnd) {
            auto &[sprite, coro] = *corosIter;
            
            if(!coro->done() && !sprite->stopScripts) {
                coro->resume();
                corosIter++;
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

    scriptBindingsStorage.reset(new ScriptManager::BindingsMap);
}

// NON-STATIC

ScriptManager::ScriptManager(const BindingsMap &bindings) {
    staticInit();
    bindScripts(bindings);
}
