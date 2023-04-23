#ifndef C_SCRATCH_UTILS_H
#define C_SCRATCH_UTILS_H

#include "value.h"
#include "string.h"

#define __str_make_bool_op(op) \
    inline bool operator op(const String &s1, const String &s2) { \
        return wcscmp(s1.data, s2.data) op 0; \
    } \
    inline bool operator op(const wchar_t * s1, const String &s2) { \
        return wcscmp(s1, s2) op 0; \
    } \
    inline bool operator op(const String &s1, const wchar_t *s2) { \
        return wcscmp(s1, s2) op 0; \
    }


const String join(const String &s1, const String &s2) {
    wchar_t * res = (wchar_t *)malloc(sizeof(wchar_t) * (s1.length + s2.length + 1));
    memcpy(res, s1.data, s1.size);
    memcpy(res + s1.length, s2.data, s2.size);

    return String(s1.length + s2.length, res);
}

const String join(const wchar_t * s1, const wchar_t * s2) {
    uint32_t len1 = wcslen(s1);
    uint32_t len2 = wcslen(s2);

    wchar_t * res = (wchar_t *)malloc((len1 + len2 + 1) << 2);

    memcpy(res, s1, len1 << 2);
    memcpy(res + len1, s2, (len2 + 1) << 2);

    return String(len1 + len2, res);
}

static inline const String join(const String &s1, double s2) {
    return join(s1.data, Value(s2).toString());
}
static inline const String join(double s1, const String &s2) {
    return join(Value(s1).toString(), s2.data);
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

static inline const String join(const wchar_t * s1, const String &s2) {
    return join(s1, s2.data);
}
static inline const String join(const String &s1, const wchar_t * s2) {
    return join(s1.data, s2);
}

static inline const String join(const String &s1, Value &s2) {
    return join(s1, s2.toString());
}
static inline const String join(Value &s1, const String &s2) {
    return join(s1.toString(), s2);
}

static inline const String join(double s1, double s2) {
    return join(Value(s1).toString(), s2);
}


static inline double lengthOf(const wchar_t * s1) {
    return wcslen(s1);
}
static inline double lengthOf(const String &s1) {
    return s1.length;
}
static inline double lengthOf(Value &s1) {
    return wcslen(s1.toString());
}
static inline double lengthOf(double s1) {
    return lengthOf(Value(s1).toString());
}


__str_make_bool_op(==)
__str_make_bool_op(>)
__str_make_bool_op(<)
__str_make_bool_op(>=)
__str_make_bool_op(<=)


static inline const String letterOf(const wchar_t * s1, uint32_t i) {
    return i > 0 && i <= wcslen(s1) ? String(s1[i-1]) : String();
}
static inline const String letterOf(const String &s1, uint32_t i) {
    return i > 0 && i <= s1.length ? String(s1.data[i-1]) : String();
}
static inline const String letterOf(Value &s1, uint32_t i) {
    return letterOf(s1.toString(), i);
}
static inline const String letterOf(double s1, uint32_t i) {
    return letterOf(Value(s1).toString(), i);
}


#endif