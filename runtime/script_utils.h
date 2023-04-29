#ifndef CSCRATCH_SCRIPT_UTILS_H
#define CSCRATCH_SCRIPT_UTILS_H

#include "config.h"

#include <cstdint>
#include <map>
#include <vector>

#include "ts_queue.h"
#include "coroutines.h"
#include "sprite.h"


typedef std::pair<Sprite*, std::vector<Coroutine(*)(Sprite*)>> SpriteScripts;
typedef std::map<int32_t, std::vector<SpriteScripts>> BindingsMap;

const constexpr int32_t ACTION_START = 0x1000'0000;
const constexpr int32_t ACTION_KEYDOWN = 0x2000'0000;

extern volatile bool shouldRun;
extern ThreadSafeQueue<Coroutine*> newActiveCoros;
extern BindingsMap scriptBindingsStorage;


void triggerScripts(int32_t action);
void bindScripts(const BindingsMap &bindings);
void startScriptsLoop();

#endif
