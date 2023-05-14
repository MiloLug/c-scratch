#ifndef CSCRATCH_STRING_H
#define CSCRATCH_STRING_H

#include <cstdint>
#include <memory>
#include <cstring>

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
    bool shouldMove = false;  // hint to not make any copies and just take the pointer
    bool isWrapper = false;  // delete is forbidden in any case, force copying on `copy` etc

    static inline String * create(const wchar_t * value) {
        String * self = (String *)malloc(sizeof(String));
        self->shouldMove = false;
        self->isWrapper = false;
        self->data = nullptr;
        self->set(value);

        return self;
    }

    static inline String * create(String & value) { return value.copy(); }
    static inline String * create(const String & value) { return value.copy(); }

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

    String(String & origin) {
        set(origin);
    }
    String(const String & origin) {
        set(origin);
    }

    void moveTo(String & destination) {
        if (!destination.isWrapper && destination.data) free(destination.data);

        destination.length = length;
        destination.size = size;
        destination.data = data;
        destination.shouldMove = shouldMove;
        destination.isWrapper = isWrapper;

        data = nullptr;
    }

    String * copy() {
        String * copy = (String *)malloc(sizeof(String));
        copy->shouldMove = false;
        copy->isWrapper = false;
        copy->data = nullptr;
        copy->set(*this);

        return copy;
    }

    String * copy() const {
        String * copy = (String *)malloc(sizeof(String));
        copy->shouldMove = false;
        copy->isWrapper = false;
        copy->data = nullptr;
        copy->set(*this);

        return copy;
    }

    void set(const String & origin) {
        length = origin.length;
        size = origin.size;
        if (origin.data) {
            data = (wchar_t *)realloc((void *)data, origin.size);
            memcpy(data, origin.data, origin.size);
        }
    }

    void set(String & origin) {
        if (origin.shouldMove && !origin.isWrapper) {
            origin.moveTo(*this);
            return;
        }

        length = origin.length;
        size = origin.size;
        if (origin.data) {
            data = (wchar_t *)realloc((void *)data, origin.size);
            memcpy(data, origin.data, origin.size);
        }
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