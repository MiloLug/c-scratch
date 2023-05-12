#include "time.h"

#include <chrono>

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;


volatile int64_t currentTime =
    duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
volatile int64_t programTime = 0;
volatile int64_t timerStartTime = 0;
int64_t programStartTime = currentTime;


void updateProgramTime() {
    currentTime =
        duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
    programTime = currentTime - programStartTime;
}

double __getTimerS() { return time_nsToS(programTime - timerStartTime); }

void resetTimer() { timerStartTime = programTime; }
