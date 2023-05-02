#ifndef CSCRATCH_SCRIPT_MANAGER_H
#define CSCRATCH_SCRIPT_MANAGER_H

#include "config.h"

#include <cstdint>
#include <map>
#include <vector>

#include "ts_queue.h"
#include "coroutines.h"
#include "sprite.h"


constexpr uint32_t ACTION_START = 0x1000'0000;
constexpr uint32_t ACTION_KEYDOWN = 0x2000'0000;


class ScriptManager {
public:
    typedef std::pair<Sprite*, std::vector<Coroutine(*)(Sprite*)>> SpriteScripts;
    typedef std::map<uint32_t, std::vector<SpriteScripts>> BindingsMap;

protected:
    static ThreadSafeQueue<Coroutine*> newActiveCoros;
    static BindingsMap scriptBindingsStorage;

public:
    static volatile bool shouldRun;
    
    static void triggerScripts(uint32_t action);
    static void bindScripts(const BindingsMap &bindings);
    static void startScriptsLoop();
};

#endif
