#ifndef CSCRATCH_TIME_H
#define CSCRATCH_TIME_H

#include <chrono>


extern volatile int64_t currentTime;
extern volatile int64_t programTime;
extern volatile int64_t timerStartTime;


#define time_sToNS(x) (int64_t)((x) * 1e9)
#define time_nsToS(x) ((double)(x) / 1e9)


void updateProgramTime();
double __getTimerS();
#define timer (__getTimerS())
void resetTimer();

#endif
