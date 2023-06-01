#ifndef CSCRATCH_DEBUG_H
#define CSCRATCH_DEBUG_H

#include "sdl.h"
#include "value.h"

#include <numeric>
#include <string>
#include <vector>

void showDebugConsole();


template<class... Args>
void csPrint(Args &&... _args) {
    std::vector<std::wstring> strings = {ArgT(_args).toString()...};

    wprintf(
        L"%ls\n",
        std::accumulate(strings.begin(), strings.end(), std::wstring{}).c_str()
    );
}

void printDebugInfo(ScratchSDLWindow & window);

#endif
