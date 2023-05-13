#ifndef CSCRATCH_STRING_H
#define CSCRATCH_STRING_H

#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>

/*
 * This is just an inner container for wchar strings.
 * !!! It's generally not supposed to be stored on stack !!!
 * 
 * Some use cases:
 * - Returning value from a function. You can allocate a string and then use:
 *   return String(length, stringPointer, true)
 *   so the data won't be copied when assigning to a variable
 * 
 * - Storing a string as a pointer to String:
 *   String::create(L"test test test")
 */
class String {
protected:
    static constexpr const wchar_t * emptyString = L"";

public:
    uint64_t length = 0;  // number of chars
    uint64_t size = 0;  // memory taken in bytes
    wchar_t * data = NULL;
    bool shouldMove = false;  // hint to not make any copies and just take the pointer
    bool isWrapper = false;  // delete is forbidden in any case, force copying on `copy` etc

    static inline String * create(const wchar_t * value) {
        String * self = (String *)malloc(sizeof(String));
        self->shouldMove = false;
        self->isWrapper = false;
        self->data = NULL;
        self->set(value);

        return self;
    }

    static inline String * create(String & value) { return value.copy(); }

    static inline double __parseIntNum(const wchar_t * str, uint8_t base, bool * isNumRet) {
        wchar_t * tmp = nullptr;
        double t = wcstol(str, &tmp, base);

        while (isspace(*(tmp))) tmp++;

        if (tmp[0] == L'\0') {
            *isNumRet = true;
            return t;
        }

        return 0;
    }

    static inline double __parseDecNum(const wchar_t * str, bool * isNumRet) {
        wchar_t * tmp = nullptr;
        double t = wcstod(str, &tmp);

        while (isspace(*(tmp))) tmp++;

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
        uint64_t len,
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

    /*Create an empty string*/
    constexpr String():
        length(0),
        data((wchar_t *)emptyString),
        size(1 << 2),
        shouldMove(true),
        isWrapper(true) {}

    /*Create a string repeating the symbol from `sym`*/
    String(wchar_t sym, uint64_t _length, bool _shouldMove = false, bool _isWrapper = false):
        length(_length),
        data((wchar_t *)malloc((_length + 1) << 2)),
        size((_length + 1) << 2),
        shouldMove(_shouldMove),
        isWrapper(_isWrapper) {
        if (_length == 1)
            data[0] = sym;
        else
            while (_length > 0) data[--_length] = sym;

        data[length] = L'\0';
        shouldMove = _shouldMove;
    }

    /*Wrap and existing string*/
    constexpr String(
        uint64_t _length, wchar_t * _data, bool _shouldMove = false, bool _isWrapper = false
    ):
        length(_length),
        data(_data),
        size((_length + 1) << 2),
        shouldMove(_shouldMove),
        isWrapper(_isWrapper) {}

    /*For some edgy cases, don't use on yourself!!!*/
    String(String & origin) {
        if (origin.shouldMove)
            origin.moveTo(*this);
        else
            set(origin);
    }

    void moveTo(String & destination) {
        if (!destination.isWrapper && destination.data) free(destination.data);

        destination.length = length;
        destination.size = size;
        destination.data = data;
        destination.shouldMove = shouldMove;
        destination.isWrapper = isWrapper;

        data = NULL;
    }

    String * copy() {
        String * copy = (String *)malloc(sizeof(String));
        copy->shouldMove = false;
        copy->isWrapper = false;
        copy->data = NULL;
        copy->set(*this);

        return copy;
    }

    void set(String & origin) {
        if (origin.shouldMove && !origin.isWrapper) {
            origin.moveTo(*this);
            return;
        }

        length = origin.length;
        size = origin.size;
        data = (wchar_t *)realloc((void *)data, origin.size);

        if (data) memcpy(data, origin.data, origin.size);
    }

    void set(const wchar_t * value) {
        length = wcslen(value);
        size = (length + 1) << 2;
        data = (wchar_t *)realloc((void *)data, size);

        if (data) memcpy(data, value, size);
    }

    constexpr void clean() {
        if (data && !isWrapper) free(data);
    }

    constexpr operator const wchar_t *() const { return data; }

    operator double() const { return strToNum(data, length); }

    // inline bool

    constexpr ~String() { clean(); }
};


#endif