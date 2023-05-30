#ifndef CSCRATCH_MESSAGE_MANAGER_H
#define CSCRATCH_MESSAGE_MANAGER_H

#include "actions.h"
#include "runtime/control_flow.h"
#include "runtime/coroutines.h"
#include "runtime/utils.h"
#include "runtime/value.h"
#include "script_manager.h"

#include <unordered_map>
#include <vector>


class Messages {
public:
    struct Context: public ::Context {
        uint64_t counter = 0;
    };

    struct Handler {
        Context * ctx;
        constexpr Handler(Context * _ctx): ctx{_ctx} {}
        constexpr Handler(::Context * _ctx): ctx{(Context *)_ctx} {}
        ~Handler() {
            if (ctx != nullptr) ctx->counter--;
        }
    };

    static void broadcast(IntegerT auto msgHash) {
        ScriptManager::triggerScripts(ACTION_MESSAGE | (uint64_t)msgHash);
    }
    static void broadcast(OneOfT<const wchar_t> auto * restrict__ msg) {
        ScriptManager::triggerScripts(ACTION_MESSAGE | strToActionParam(msg));
    }
    static void broadcast(Arg value) {
        ScriptManager::triggerScripts(ACTION_MESSAGE | strToActionParam(value.toString()));
    }


    static Coroutine broadcastWait(IntegerT auto msgHash) {
        Context ctx;
        auto running = ScriptManager::triggerScripts(ACTION_MESSAGE | (uint64_t)msgHash, &ctx);

        if (running != 0) {
            ctx.counter = running;
            wait_until(ctx.counter == 0);
        }

        co_return;
    }
    static Coroutine broadcastWait(OneOfT<const wchar_t> auto * restrict__ msg) {
        return broadcastWait(strToActionParam(msg));
    }
    static Coroutine broadcastWait(Arg value) {
        return broadcastWait(strToActionParam(value.toString()));
    }
};

#endif
