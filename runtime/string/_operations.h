#ifndef CSCRATCH_STRING_UTILS_OPERATIONS_H
#define CSCRATCH_STRING_UTILS_OPERATIONS_H

#include "_bmh_search.h"
#include "runtime/utils.h"
#include "runtime/value.h"
#include "string.h"


#define __str_make_bool_op(op)                                                                     \
    inline bool operator op(OneOfT<const wchar_t> auto * restrict__ s1, String && s2) {            \
        return wcscmp(s1, s2.data) op 0;                                                           \
    }                                                                                              \
    inline bool operator op(String && s1, String && s2) { return wcscmp(s1.data, s2.data) op 0; }  \
    inline bool operator op(String && s1, OneOfT<const wchar_t> auto * restrict__ s2) {            \
        return wcscmp(s1.data, s2) op 0;                                                           \
    }                                                                                              \
    inline bool operator op(String && s1, NumberT auto s2) {                                       \
        return wcscmp(s1.data, Const(s2).getNumberStr()) op 0;                                     \
    }                                                                                              \
    inline bool operator op(NumberT auto s1, String && s2) {                                       \
        return wcscmp(Const(s1).getNumberStr(), s2.data) op 0;                                     \
    }


static constexpr String __str_toTmpString(OneOfT<const wchar_t> auto * restrict__ s1) {
    return String(std::char_traits<wchar_t>::length(s1), (wchar_t *)s1, true);
}
static constexpr const String & __str_toTmpString(const String & s1) { return s1; }
static inline String __str_toTmpString(const Const & s1) {
    if (s1.type == Const::Type::STRING) return String(s1.string->length, s1.string->data, true);

    s1.getNumberStr();
    return String(s1.numberStrLen, s1.numberStrTmp, true);
}
static inline String __str_toTmpString(NumberT auto s1) {
    Const tmp(s1, nullptr);
    wchar_t * tmpStr = tmp.getNumberStr();
    tmp.numberStrTmp = NULL;
    return String(tmp.numberStrLen, tmpStr);
}


__str_make_bool_op(==);
__str_make_bool_op(>);
__str_make_bool_op(<);
__str_make_bool_op(>=);
__str_make_bool_op(<=);


static String join(const String & s1, const String & s2) {
    wchar_t * res = (wchar_t *)malloc((s1.length + s2.length + 1) << 2);
    memcpy(res, s1.data, s1.size);
    memcpy(res + s1.length, s2.data, s2.size);

    return String(s1.length + s2.length, res);
}
static inline String join(auto && s1, auto && s2) {
    return join(__str_toTmpString(s1), __str_toTmpString(s2));
}


static constexpr double lengthOf(const String & s1) { return s1.length; }
static inline double lengthOf(auto && s1) { return lengthOf(__str_toTmpString(s1)); }


static inline String letterOf(const String & s1, uint64_t i) {
    return i > 0 && i <= s1.length ? String(s1.data[i - 1], 1) : String();
}
static inline String letterOf(auto && s1, uint64_t i) { return letterOf(__str_toTmpString(s1), i); }


static constexpr String operator""_S(const wchar_t * str, size_t size) {
    if (size != 0 && str[0] != L'\0') return String(size, (wchar_t *)str, true);
    return String();
}


static bool strContains(const String & haystack, const String & needle) {
    return BMHSearch::findIn(haystack.data, haystack.length, needle.data, needle.length)
        != haystack.length;
}
static inline bool strContains(auto && haystack, auto && needle) {
    return strContains(__str_toTmpString(haystack), __str_toTmpString(needle));
}

#endif