#ifndef C_SCRATCH_UTILS_H
#define C_SCRATCH_UTILS_H

#include "value.h"
#include "string.h"

#define __str_make_bool_op(op) \
    inline bool operator op(String &&s1, String &&s2) { \
        return wcscmp(s1.data, s2.data) op 0; \
    } \
    inline bool operator op(const wchar_t * s1, String &&s2) { \
        return wcscmp(s1, s2.data) op 0; \
    } \
    inline bool operator op(String &&s1, const wchar_t *s2) { \
        return wcscmp(s1.data, s2) op 0; \
    } \
    inline bool operator op(String &&s1, double s2) { \
        return wcscmp(s1.data, toTmpString(s2).data) op 0; \
    } \
    inline bool operator op(double s1, String &&s2) { \
        return wcscmp(toTmpString(s1).data, s2.data) op 0; \
    } \
    inline bool operator op(String &&s1, int s2) { \
        return wcscmp(s1.data, toTmpString(s2).data) op 0; \
    } \
    inline bool operator op(int s1, String &&s2) { \
        return wcscmp(toTmpString(s1).data, s2.data) op 0; \
    }


static constexpr String toTmpString(const wchar_t * s1) {
    return String(std::char_traits<wchar_t>::length(s1), (wchar_t *)s1, true, true);
}
static constexpr const String &toTmpString(const String &s1) {
    return s1;
}
static constexpr String toTmpString(Value &s1) {
    if (s1.type == Value::Type::STRING)
        return String(s1.string->length, s1.string->data, true, true);

    return String(s1.numberStrSize - 1, s1.getNumberStr(), true, true);
}
static constexpr String toTmpString(double s1) {
    Value tmp(s1);
    wchar_t * tmpStr = tmp.getNumberStr();
    tmp.numberStrTmp = NULL;
    return String(tmp.numberStrSize-1, tmpStr, true);
}


__str_make_bool_op(==)
__str_make_bool_op(>)
__str_make_bool_op(<)
__str_make_bool_op(>=)
__str_make_bool_op(<=)


static constexpr String join(const String &s1, const String &s2) {
    wchar_t * res = (wchar_t *)malloc((s1.length + s2.length + 1) << 2);
    memcpy(res, s1.data, s1.size);
    memcpy(res + s1.length, s2.data, s2.size);

    return String(s1.length + s2.length, res, true);
}
template<typename T1, typename T2>
static constexpr String join(T1 &&s1, T2 &&s2) {
    return join(toTmpString(s1), toTmpString(s2));
}


static constexpr double lengthOf(const String &s1) {
    return s1.length;
}
template<typename T>
static constexpr double lengthOf(T &&s1) {
    return lengthOf(toTmpString(s1));
}


static constexpr String letterOf(const String &s1, uint64_t i) {
    return i > 0 && i <= s1.length ? String(s1.data[i-1], 1, true) : String();
}
template<typename T>
static constexpr String letterOf(T &&s1, uint64_t i) {
    return letterOf(toTmpString(s1), i);
}


static constexpr String operator ""_S(const wchar_t * str, size_t size) {
    if (size != 0)
        return String(size, (wchar_t *)str, true, true);
    return String();
} 


#endif
