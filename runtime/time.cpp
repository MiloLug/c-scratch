#include "time.h"

#include <chrono>

using std::chrono::duration;
using std::chrono::high_resolution_clock;


volatile double programTime = 0;
volatile double timeStartTime = 0;
auto programStartTime = high_resolution_clock::now();


void updateProgramTime() {
    programTime = duration<double>(high_resolution_clock::now() - programStartTime).count();
}

double __getTime() { return programTime - timeStartTime; }

void resetTimer() { timeStartTime = programTime; }
