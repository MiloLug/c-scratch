#ifndef CSCRATCH_SCRIPTS_BASE_H
#define CSCRATCH_SCRIPTS_BASE_H

#include "script_manager.h"

class ScriptsBase {
public:
    SpriteBase * __sprite;

    ScriptsBase(SpriteBase * _sprite): __sprite{_sprite} {}

    void on(uint64_t action, const ScriptManager::CoroFunction & fn) {
        ScriptManager bind(__sprite, action, fn);
    }
};

#endif
