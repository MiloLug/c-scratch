#ifndef CSCRATCH_TIME_H
#define CSCRATCH_TIME_H

extern volatile double programTime;
extern volatile double timeStartTime;

void updateProgramTime();
double __getTime();
#define timer (__getTime())
void resetTimer();

#endif
