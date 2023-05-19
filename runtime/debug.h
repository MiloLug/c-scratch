#ifndef CSCRATCH_DEBUG_H
#define CSCRATCH_DEBUG_H

#include "sdl.h"
#include "value.h"

void windowsTryShowConsole();

void cs_print(Arg value, bool quoted = false);
void cs_print(const wchar_t * name, Arg value, bool quoted = false);

void printDebugInfo(ScratchSDLWindow & window);

#endif
