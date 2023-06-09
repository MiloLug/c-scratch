#ifndef CSCRATCH_SCRIPT_MANAGER_H
#define CSCRATCH_SCRIPT_MANAGER_H

#include "runtime/coroutines.h"
#include "runtime/ts_queue.h"
#include "runtime/utils.h"
#include "should_run.h"

#include <cstdint>
#include <map>
#include <memory>
#include <vector>


class SpriteBase;


class ScriptManager {
public:
    typedef Coroutine (*CoroFunction)(Context *);
    typedef std::pair<SpriteBase *, Coroutine *> CoroContainer;
    typedef std::pair<SpriteBase *, std::vector<CoroFunction>> SpriteScripts;
    typedef std::map<uint64_t, std::vector<SpriteScripts>> BindingsMap;
    typedef std::map<uint64_t, std::vector<CoroFunction>> SimpleBindingsMap;

protected:
    static ThreadSafeQueue<CoroContainer> newActiveCoros;
    static constinit BindingsMap * scriptBindingsStorage;

    bool isInitializer = false;

public:
    static uint32_t triggerScripts(uint64_t action, Context * ctx = nullptr);
    static void manageCoroutine(SpriteBase * sprite, Coroutine * coroutine);
    static void bindScripts(const BindingsMap & bindings);
    static void bindScripts(SpriteBase * sprite, const SimpleBindingsMap & bindings);
    static void bindScripts(SpriteBase * sprite, uint64_t action, const CoroFunction & function);
    static void startScriptsLoop();

    static void staticInit();

    ScriptManager();
    ScriptManager(const BindingsMap & bindings);
    ScriptManager(SpriteBase * sprite, const SimpleBindingsMap & bindings);
    ScriptManager(SpriteBase * sprite, uint64_t action, const CoroFunction & function);
    ~ScriptManager();
};

#endif
