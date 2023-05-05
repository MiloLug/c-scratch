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


constexpr uint32_t ACTION_START = 0x1000'0000;
constexpr uint32_t ACTION_KEYDOWN = 0x2000'0000;


class ScriptManager {
public:
    typedef std::pair<SpriteBase *, Coroutine *> CoroContainer;
    typedef std::pair<SpriteBase *, std::vector<Coroutine(*)()>> SpriteScripts;
    typedef std::map<uint32_t, std::vector<SpriteScripts>> BindingsMap;

protected:
    static ThreadSafeQueue<CoroContainer> newActiveCoros;
    static constinit std::unique_ptr<BindingsMap> scriptBindingsStorage;

public:
    static volatile bool shouldRun;
    
    static void triggerScripts(uint32_t action);
    static void bindScripts(const BindingsMap &bindings);
    static void startScriptsLoop();

    static void staticInit();

    ScriptManager(const BindingsMap &bindings);
};

#endif
