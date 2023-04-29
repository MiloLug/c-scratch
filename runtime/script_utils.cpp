#include "script_utils.h"
#include <list>
#include "coroutines.h"


volatile bool shouldRun = true;
ThreadSafeQueue<Coroutine*> newActiveCoros;
BindingsMap scriptBindingsStorage;


void triggerScripts(int32_t action) {
    auto actionBindings = scriptBindingsStorage.find(action);
    if (actionBindings != scriptBindingsStorage.end()) {
        for (auto &[sprite, coros] : actionBindings->second) {
            for (auto &coro : coros) {
                newActiveCoros.push(new Coroutine(coro(sprite)));
            }
        }
    }
}

void bindScripts(const BindingsMap &bindings) {
    for (auto &[action, actionSprites] : bindings) {
        auto globalActionBindings = scriptBindingsStorage.find(action);

        if (globalActionBindings != scriptBindingsStorage.end()) {
            auto &globalActionSprites = (*globalActionBindings).second;

            for (auto &sprite : actionSprites) {
                globalActionSprites.push_back(sprite);
            }
        } else {
            scriptBindingsStorage[action] = actionSprites;
        }
    }
}

void startScriptsLoop() {
    std::list<Coroutine*> activeCoros;
    Coroutine * newCoroutine;

    triggerScripts(ACTION_START);

    #ifndef ENABLE_TURBO
        const int clocks_per_frame = CLOCKS_PER_SEC / NON_TURBO_CALCULATION_FPS;
        std::clock_t previous_time = std::clock();
    #else
        #ifndef ENABLE_UNSAFE_NO_LOCKS
            int32_t unlockCounter = 0;
            Pen::pixels.take();
        #endif
    #endif

    while (shouldRun) {
        #ifndef ENABLE_TURBO
            if (int(std::clock() - previous_time) < clocks_per_frame)
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
        

        while((newCoroutine = newActiveCoros.pop()) != NULL) {
            activeCoros.push_back(newCoroutine);
        }
        
        auto corosIter = activeCoros.begin();
        auto corosEnd = activeCoros.end();
        while(corosIter != corosEnd) {
            auto &coro = *corosIter;
            
            if(shouldRun = !coro->done()) {
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