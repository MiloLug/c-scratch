#ifndef CSCRATCH_MATH_COMMON_H
#define CSCRATCH_MATH_COMMON_H

#include <cstdint>
#include <cfloat>
#include "runtime/utils.h"


#define GET_HIGH_WORD(hi, d)                                                                       \
    {                                                                                              \
        union {                                                                                    \
            double f;                                                                              \
            uint64_t i;                                                                            \
        } __u;                                                                                     \
        __u.f = (d);                                                                               \
        (hi) = __u.i >> 32;                                                                        \
    }

#ifndef EPS
constexpr double EPS = 2.22044604925031308084726333618164062e-16L;
#endif

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
const constexpr double M_PI_2 = 3.1415926535897932384626433832795 / 2.0;
#endif

#ifndef M_PI_3
const constexpr double M_PI_3 = 3.1415926535897932384626433832795 / 3.0;
#endif

#ifndef M_PI_4
const constexpr double M_PI_4 = 3.1415926535897932384626433832795 / 4.0;
#endif


const constexpr double M_RAD = 3.1415926535897932384626433832795 / 180.0;

const constexpr int32_t INT32_BITS = 8 * sizeof(int32_t);
const constexpr int32_t INT32_OVERFLOW = -2147483648;

#ifndef INT32_MAX
const constexpr uint32_t INT32_MAX = (1 << (INT32_BITS - 1)) - 1;
#endif

#ifndef MAX
force_inline__ auto MAX(NumberT auto a, NumberT auto b) {
    auto __a = (a);
    auto __b = (b);
    return __a > __b ? __a : __b;
}
#endif

#ifndef MIN
force_inline__ auto MIN(NumberT auto a, NumberT auto b) {
    auto __a = (a);
    auto __b = (b);
    return __a < __b ? __a : __b;
}
#endif

#define MAX_UNSAFE(a, b) ((a) > (b) ? (a) : (b))
#define MIN_UNSAFE(a, b) ((a) < (b) ? (a) : (b))


#define DISTANCE0_UNSAFE(x, y) sqrt((x) * (x) + (y) * (y))


#endif
