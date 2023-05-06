#ifndef CSCRATCH_SCRIPT_MANAGER_H
#define CSCRATCH_SCRIPT_MANAGER_H

#include "config.h"

#include <cstdint>
#include <map>
#include <vector>
#include <memory>

#include "ts_queue.h"
#include "coroutines.h"
#include "sprite_base.h"
#include "actions.h"


class ScriptManager {
public:
    typedef std::pair<SpriteBase *, Coroutine *> CoroContainer;
    typedef std::pair<SpriteBase *, std::vector<Coroutine(*)(Context *)>> SpriteScripts;
    typedef std::map<uint64_t, std::vector<SpriteScripts>> BindingsMap;

protected:
    static ThreadSafeQueue<CoroContainer> newActiveCoros;
    static constinit std::unique_ptr<BindingsMap> scriptBindingsStorage;

public:
    static volatile bool shouldRun;
    
    static uint64_t triggerScripts(uint64_t action, Context * ctx = nullptr);
    static void bindScripts(const BindingsMap &bindings);
    static void startScriptsLoop();

    static void staticInit();

    ScriptManager(const BindingsMap &bindings);
};

#endif
