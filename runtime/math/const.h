#ifndef CSCRATCH_CONST_H
#define CSCRATCH_CONST_H

#include <cstdint>
#include <cmath>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_4
    const constexpr double M_PI_4 = 3.1415926535897932384626433832795/4.0;
#endif

#ifndef M_PI_3
    const constexpr double M_PI_3 = 3.1415926535897932384626433832795/3.0;
#endif

const constexpr double M_RAD = M_PI / 180.0;

const constexpr int32_t INT32_BITS = 8 * sizeof(int32_t);
const constexpr int32_t INT32_OVERFLOW = 2147483648;

#ifndef INT32_MAX
    const constexpr uint32_t INT32_MAX = ((1<<(INT32_BITS-1))-1);
#endif


#endif
