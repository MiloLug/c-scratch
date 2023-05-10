#ifndef CSCRATCH_STRING_H
#define CSCRATCH_STRING_H

#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <string>

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

    static String * create(const wchar_t * value) {
        String * self = (String *)malloc(sizeof(String));
        self->shouldMove = false;
        self->isWrapper = false;
        self->data = NULL;
        self->set(value);

        return self;
    }

    static String * create(String & value) { return value.copy(); }

    static double __parseHexNum(double & sign, const wchar_t * str) {
        if (str[0] == L'0') {
            str += 2;
            const wchar_t * tmpStr = str - 1;

            while (iswxdigit(*(++tmpStr)))
                ;
            tmpStr--;
            while (isspace(*(++tmpStr)))
                ;
            if (*tmpStr == L'\0')
                return wcstol(str, nullptr, 16) * sign;
        }

        return 0;
    }

    static double __parseOctNum(double & sign, const wchar_t * str) {
        if (str[0] == L'0') {
            str += 2;
            const wchar_t * tmpStr = str;

            while (*tmpStr >= L'0' && *tmpStr <= L'7') tmpStr++;
            tmpStr--;
            while (isspace(*(++tmpStr)))
                ;
            if (*tmpStr == L'\0')
                return wcstol(str, nullptr, 8) * sign;
        }

        return 0;
    }

    static double __parseBinNum(double & sign, const wchar_t * str) {
        if (str[0] == L'0') {
            str += 2;
            const wchar_t * tmpStr = str;

            while (*tmpStr == L'0' || *tmpStr == L'1') tmpStr++;
            tmpStr--;
            while (isspace(*(++tmpStr)))
                ;
            if (*tmpStr == L'\0')
                return wcstol(str, nullptr, 2) * sign;
        }

        return 0;
    }

    static double __parseDecNum(double & sign, const wchar_t * str) {
        bool hasNumbers = false;
        bool hasExp = false;
        wchar_t tmp;
        const wchar_t * tmpStr = str - 1;

        while (iswdigit(*(++tmpStr)))
            ;
        hasNumbers = tmpStr - str;

        if (*tmpStr == L'.') {
            while (iswdigit(*(++tmpStr)))
                ;
            hasNumbers = tmpStr - str - 1;
        }

        if (!hasNumbers)
            return 0;

        if (*tmpStr == L'E' || *tmpStr == L'e') {
            tmp = *(++tmpStr);
            if (!iswdigit(tmp) && ((tmp != L'-' && tmp != L'+') || !iswdigit(*(++tmpStr))))
                return 0;
            while (iswdigit(*(++tmpStr)))
                ;
        }

        tmpStr--;
        while (isspace(*(++tmpStr)))
            ;

        if (*tmpStr == L'\0')
            return wcstod(str, nullptr) * sign;

        return 0;
    }

    static double strToNum(const wchar_t * str, uint16_t len) {
        if (len > 326)
            return 0;  // > -MAX_DBL len
        while (iswspace(*str)) str++;

        bool hasMinus = str[0] == L'-';
        if (hasMinus || str[0] == L'+') {
            str++;
        }
        double sign = hasMinus ? -1 : 1;

        if (len == 0)
            return 0;

        switch (str[1]) {
            case L'x':
                return __parseHexNum(sign, str);
            case L'o':
                return __parseOctNum(sign, str);
            case L'b':
                return __parseBinNum(sign, str);
            case L'n':
                return wcsncmp(str, L"Infinity", 9) == 0 || (str[0] == L'i' && str[2] == 'f')
                    ? std::numeric_limits<double>::infinity() * sign
                    : 0;
            default:
                return iswdigit(str[0]) || str[0] == L'.' ? __parseDecNum(sign, str) : 0;
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

    /*For some edgy cases*/
    String(String & origin) {
        if (origin.shouldMove)
            origin.moveTo(*this);
        else
            set(origin);
    }

    void moveTo(String & destination) {
        if (!destination.isWrapper && destination.data)
            free(destination.data);

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

        if (data)
            memcpy(data, origin.data, origin.size);
    }

    void set(const wchar_t * value) {
        length = std::char_traits<wchar_t>::length(value);
        size = (length + 1) << 2;
        data = (wchar_t *)realloc((void *)data, size);

        if (data)
            memcpy(data, value, size);
    }

    constexpr void clean() {
        if (data && !isWrapper)
            free(data);
    }

    constexpr operator const wchar_t *() const { return data; }

    operator double() const { return strToNum(data, length); }

    // inline bool

    constexpr ~String() { clean(); }
};


#endif