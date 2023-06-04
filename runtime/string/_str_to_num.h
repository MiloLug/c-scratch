#ifndef STRING_TO_NUMBER_H
#define STRING_TO_NUMBER_H

#include <cstdint>
#include <cwchar>
#include <cwctype>
#include <limits>


static inline double __parseIntNum(const wchar_t * str, uint8_t base, bool * isNumRet) {
    wchar_t * tmp = nullptr;
    double t = wcstol(str, &tmp, base);

    while (iswspace(*(tmp))) tmp++;

    if (tmp[0] == L'\0') {
        *isNumRet = true;
        return t;
    }

    return 0;
}

static inline double __parseDecNum(const wchar_t * str, bool * isNumRet) {
    wchar_t * tmp = nullptr;
    double t = wcstod(str, &tmp);

    while (iswspace(*(tmp))) tmp++;

    if (tmp[0] == L'\0') {
        *isNumRet = true;
        return t;
    }

    return 0;
}

static inline double __parseInfinity(const wchar_t * str, bool * isNumRet) {
    if (wcscmp(str, L"Infinity") == 0 || (str[0] == L'i' && str[2] == 'f' && str[3] == L'\0')) {
        *isNumRet = true;
        return std::numeric_limits<double>::infinity();
    }

    return 0;
}

static double strToNum(
    const wchar_t * str,
    uint32_t len,
    bool * isNumRet = nullptr,
    uint8_t * baseRet = nullptr,
    bool noSpace = false
) {
    bool tmpIsNum;
    isNumRet = isNumRet ? isNumRet : &tmpIsNum;
    *isNumRet = false;

    uint8_t tmpBase;
    baseRet = baseRet ? baseRet : &tmpBase;
    *baseRet = 10;

    if (noSpace && (iswspace(*str) || iswspace(str[len - 1]))) {
        // Just forbid making the ret value true
        isNumRet = &tmpIsNum;
    }

    while (iswspace(*str)) str++, len--;
    if (len > 326 || len < 1)  // > -MAX_DBL len
        return 0;

    const double sign = str[0] == L'-' ? -1 : 1;
    if (str[0] == L'-' || str[0] == L'+') {
        str++;
        len--;
    }

    if (len == 0) return 0;

    switch (str[1]) {
        case L'x':
            *baseRet = 16;
            return sign * __parseIntNum(str + 2, 16, isNumRet);
        case L'o':
            *baseRet = 8;
            return sign * __parseIntNum(str + 2, 8, isNumRet);
        case L'b':
            *baseRet = 2;
            return sign * __parseIntNum(str + 2, 2, isNumRet);
        case L'n':
            return sign * __parseInfinity(str, isNumRet);
        default:
            return sign * __parseDecNum(str, isNumRet);
    }

    return 0;
}


#endif
