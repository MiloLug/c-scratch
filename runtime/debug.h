#ifndef CSCRATCH_DEBUG_H
#define CSCRATCH_DEBUG_H

#include "sdl.h"
#include "value.h"

void showDebugConsole();

void csPrint(Arg value, bool quoted = false);
void csPrint(const wchar_t * name, Arg value, bool quoted = false);

void printDebugInfo(ScratchSDLWindow & window);

#endif
