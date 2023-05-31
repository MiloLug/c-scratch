#ifndef CSCRATCH_MATH_NUMBER_H
#define CSCRATCH_MATH_NUMBER_H

#include "config.h"
#include <limits>
#include <string>


#ifdef USE_VALUE_FLOAT
    typedef float storage_number_t;
    constexpr int NT_PRECISION_CORRECTION = 0;
#else
    typedef double storage_number_t;
    constexpr int NT_PRECISION_CORRECTION = -1;
#endif

constexpr auto NT_INF = std::numeric_limits<storage_number_t>::infinity();
constexpr auto NT_NEG_INF = - std::numeric_limits<storage_number_t>::infinity();
constexpr auto NT_MAX_PRECISION = std::numeric_limits<storage_number_t>::digits10 + NT_PRECISION_CORRECTION;


static auto __NT_FORMAT = L"%." + std::to_wstring(NT_MAX_PRECISION) + L"f";
static auto NT_FORMAT = __NT_FORMAT.c_str();

static auto __NT_FORMAT_EXP = L"%." + std::to_wstring(NT_MAX_PRECISION) + L"e";
static auto NT_FORMAT_EXP = __NT_FORMAT_EXP.c_str();

#endif
