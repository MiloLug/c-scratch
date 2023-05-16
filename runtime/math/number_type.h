#ifndef CSCRATCH_MATH_NUMBER_H
#define CSCRATCH_MATH_NUMBER_H

#include "config.h"
#include <limits>


#ifdef USE_VALUE_FLOAT
    typedef float storage_number_t;
#else
    typedef double storage_number_t;
#endif

constexpr storage_number_t NT_INF = std::numeric_limits<storage_number_t>::infinity();
constexpr storage_number_t NT_NEG_INF = - std::numeric_limits<storage_number_t>::infinity();


#endif
