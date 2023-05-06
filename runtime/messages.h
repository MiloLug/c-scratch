#ifndef CSCRATCH_MESSAGE_MANAGER_H
#define CSCRATCH_MESSAGE_MANAGER_H

#include <unordered_map>
#include <vector>
#include "utils.h"
#include "actions.h"
#include "script_manager.h"
#include "value.h"
#include "coroutines.h"
#include "control_flow.h"


class Messages {
public:
    struct Context: public ::Context {
        uint64_t counter = 0;
    };

    struct Handler {
        Context * ctx;
        Handler(Context * _ctx): ctx{_ctx} {}
        Handler(::Context * _ctx): ctx{(Context *)_ctx} {}
        ~Handler() {
            if (ctx != nullptr) ctx->counter--;
        }
    };

    static void broadcast(uint64_t msgHash) {
        ScriptManager::triggerScripts(ACTION_MESSAGE | msgHash);
    }
    static void broadcast(const wchar_t * msg) {
        ScriptManager::triggerScripts(ACTION_MESSAGE | strToActionParam(msg));
    }
    static void broadcast(Value &&value) {
        ScriptManager::triggerScripts(ACTION_MESSAGE | strToActionParam(value.toString()));
    }


    static Coroutine broadcastWait(uint64_t msgHash) {
        Context ctx;
        auto running = ScriptManager::triggerScripts(ACTION_MESSAGE | msgHash, &ctx);

        if (running != 0) {
            ctx.counter = running;
            waitUntil(ctx.counter == 0);
        }

        co_return;
    }
    static Coroutine broadcastWait(const wchar_t * msg) {
        return broadcastWait(strToActionParam(msg));
    }
    static Coroutine broadcastWait(Value &&value) {
        return broadcastWait(strToActionParam(value.toString()));
    }
};

#endif
