#ifndef CSCRATCH_TIME_H
#define CSCRATCH_TIME_H

#include <chrono>

// UNIX timestamp in nanosecs
extern volatile int64_t currentTime;

// Time from from the program start in nanosecs
extern volatile int64_t programTime;

// Current timer's time in nanosecs
extern volatile int64_t timerStartTime;


#define time_sToNS(x) (int64_t)((x) * 1e9)
#define time_nsToS(x) ((double)(x) / 1e9)


void updateProgramTime();

// Returns current timer time in nanosecs
#define __getTimer() (programTime - timerStartTime)

// Returns current timer's time in seconds
#define timer (time_nsToS(__getTimer()))

#define resetTimer() (timerStartTime = programTime)

#endif
