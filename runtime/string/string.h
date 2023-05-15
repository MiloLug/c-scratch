#ifndef CSCRATCH_STRING_H
#define CSCRATCH_STRING_H

#include <cstdint>
#include <memory>
#include <cstring>
#include "runtime/utils.h"

#include "_str_to_num.h" 


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
    wchar_t * data = nullptr;
    bool isWrapper = false;  // delete is forbidden in any case, force copying on `copy` etc

    /*Create an empty string*/
    constexpr String():
        length{0},
        data{(wchar_t *)emptyString},
        size{1 << 2},
        isWrapper{true} {}

    /*Create a string repeating the symbol from `sym`*/
    String(wchar_t sym, uint64_t _length):
        length{_length},
        data{(wchar_t *)malloc((_length + 1) << 2)},
        size{(_length + 1) << 2} {
        if (_length == 1)
            data[0] = sym;
        else
            while (_length > 0) data[--_length] = sym;

        data[length] = L'\0';
    }

    /*Wrap and existing string*/
    constexpr String(
        uint64_t _length, wchar_t * _data, bool _isWrapper = false
    ):
        length(_length),
        data(_data),
        size((_length + 1) << 2),
        isWrapper(_isWrapper) {}

    String(OneOf<const wchar_t> auto * restrict__ _data) {
        operator=(_data);
    }
    String(String && origin) {
        length = origin.length;
        size = origin.size;
        data = origin.data;
        isWrapper = origin.isWrapper;
        origin.data = nullptr;
    }
    String(const String & origin) {
        length = origin.length;
        size = origin.size;
        if (origin.data) {
            data = (wchar_t *)malloc(origin.size);
            memcpy(data, origin.data, origin.size);
        }
    }

    String & operator=(String && origin) {
        if (!isWrapper && data) free(data);

        length = origin.length;
        size = origin.size;
        data = origin.data;
        isWrapper = origin.isWrapper;
        origin.data = nullptr;

        return *this;
    }
    String & operator=(const String & origin) {
        length = origin.length;
        size = origin.size;
        if (origin.data) {
            if (isWrapper) data = nullptr;

            data = (wchar_t *)realloc((void *)data, origin.size);
            memcpy(data, origin.data, origin.size);
        }
        isWrapper = false;

        return *this;
    }
    String & operator=(OneOf<const wchar_t> auto * value) {
        length = wcslen(value);
        size = (length + 1) << 2;
        data = (wchar_t *)realloc((void *)data, size);

        if (data) memcpy(data, value, size);

        return *this;
    }

    constexpr void clean() {
        if (data && !isWrapper) free(data);
    }

    constexpr operator const wchar_t *() const { return data; }

    operator double() const { return strToNum(data, length); }

    constexpr ~String() { clean(); }
};


#endif