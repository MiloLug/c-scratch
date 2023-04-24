#include "script_utils.h"


ThreadSafeQueue<Coroutine*> newActiveCoros;


void triggerCoroutines(int32_t action, const BindingsMap &bindings) {
    auto actionBindings = bindings.find(action);
    if (actionBindings != bindings.end()) {
        for(auto &[sprite, coros] : actionBindings->second) {
            for (auto &coro : coros) {
                newActiveCoros.push(new Coroutine(coro(sprite)));
            }
        }
    }
}
