#ifndef C_SCRATCH_UTILS_H
#define C_SCRATCH_UTILS_H

#include "value.h"
#include "string.h"


const String join(const wchar_t * s1, const wchar_t * s2) {
    uint32_t len1 = wcslen(s1);
    uint32_t len2 = wcslen(s2);

    wchar_t * res = (wchar_t *)malloc((len1 + len2 + 1) << 2);

    memcpy(res, s1, len1 << 2);
    memcpy(res + len1, s2, (len2 + 1) << 2);

    return String(len1 + len2, res);
}

const String join(const String &s1, const String &s2) {
    wchar_t * res = (wchar_t *)malloc(sizeof(wchar_t) * (s1.length + s2.length + 1));
    memcpy(res, s1.data, s1.size);
    memcpy(res + s1.length, s2.data, s2.size);

    return String(s1.length + s2.length, res);
}

static inline const String join(const wchar_t * s1, double s2) {
    return join(s1, Value(s2).toString());
}
static inline const String join(double s1, const wchar_t * s2) {
    return join(Value(s1).toString(), s2);
}

static inline const String join(const wchar_t * s1, Value &s2) {
    return join(s1, s2.toString());
}
static inline const String join(Value &s1, const wchar_t * s2) {
    return join(s1.toString(), s2);
}


#endif