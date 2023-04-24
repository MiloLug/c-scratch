#ifndef CSCRATCH_SCRIPT_UTILS_H
#define CSCRATCH_SCRIPT_UTILS_H

#include <cstdint>
#include <map>
#include <vector>

#include "ts_queue.h"
#include "coroutines.h"
#include "sprite.h"


const constexpr int32_t ACTION_START = 0x1000'0000;
const constexpr int32_t ACTION_KEYDOWN = 0x2000'0000;

extern ThreadSafeQueue<Coroutine*> newActiveCoros;

typedef std::pair<Sprite*, std::vector<Coroutine(*)(Sprite*)>> SpriteScripts;
typedef std::map<int32_t, std::vector<SpriteScripts>> BindingsMap;


void triggerCoroutines(int32_t action, const BindingsMap &bindings);


#endif
