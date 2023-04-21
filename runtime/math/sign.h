#ifndef CSCRATCH_SIGN_H
#define CSCRATCH_SIGN_H


int mSign(double val) {
    return (0.0 < val) - (val < 0.0);
}

int mSign(int val) {
    return (0 < val) - (val < 0);
}


#endif
