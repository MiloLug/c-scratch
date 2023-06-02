#ifndef CSCRATCH_EXP_H
#define CSCRATCH_EXP_H

#include <memory>
#include <cmath>
#include <cstdint>
#include <type_traits>
#include <limits>
#include "runtime/utils.h"

constexpr double cxPow(double base, int32_t x) {
    if (x == 0) return 1;
    if (x == 1 || x == -1) return base;

    bool neg = x < 0;
    x = x < 0 ? -x : x;

    double res = base;

    while (--x) {
        res *= base;
    }
    return neg ? 1. / res : res;
}

namespace __exp_tools {
    template<int64_t Base, int64_t N>
    struct PowTable {
        const int64_t n = N;
        const int64_t base = Base;
        double data[N * 2 + 1];

        constexpr PowTable(): data{} {
            data[N] = 1;
            for (int64_t i = 1; i <= N; i++) {
                data[N - i] = cxPow(Base, -i);
                data[N + i] = cxPow(Base, i); 
            }
        }
    };

    constexpr auto powTable10 = PowTable<10, std::numeric_limits<double>::max_exponent10>();
};

static double mExp10(double x) {
    if (x > std::numeric_limits<double>::max_exponent10)
        return std::numeric_limits<double>::infinity();
    if (x < -std::numeric_limits<double>::max_exponent10) return 0;

    if (x == (int64_t)x) {
        return __exp_tools::powTable10.data[
            __exp_tools::powTable10.n + (int64_t)x
        ];
    }
    return pow(10., x);
}

static float mExp10(float x) {
    if (x > std::numeric_limits<float>::max_exponent10)
        return std::numeric_limits<float>::infinity();
    if (x < -std::numeric_limits<float>::max_exponent10) return 0;

    if (x == (int64_t)x) {    
        return __exp_tools::powTable10.data[
            __exp_tools::powTable10.n + (int64_t)x
        ];
    }
    return pow(10.f, x);
}

static inline double mExp10(int64_t x) {
    if (x > std::numeric_limits<double>::max_exponent10)
        return std::numeric_limits<double>::infinity();
    if (x < -std::numeric_limits<double>::max_exponent10) return 0;

    return __exp_tools::powTable10.data[__exp_tools::powTable10.n + x];
}

#endif
