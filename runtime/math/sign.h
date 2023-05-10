#ifndef CSCRATCH_SIGN_H
#define CSCRATCH_SIGN_H


static inline int sign(double val) { return (0.0 < val) - (val < 0.0); }

static inline int sign(int val) { return (0 < val) - (val < 0); }


#endif
