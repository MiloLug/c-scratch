#ifndef CSCRATCH_STRING_H
#define CSCRATCH_STRING_H

#include <memory>
#include <cstring>
#include <cstdint>


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
    static const wchar_t * emptyString;
public:
    uint64_t length = 0;  // number of chars
    uint64_t size = 0;  // memory taken in bytes
    wchar_t * data = NULL;
    bool shouldMove = false;  // hint to not make any copies and just take the pointer
    bool isWrapper = false;  // delete is forbidden in any case, force copying on `copy` etc

    static String * create(const wchar_t * value) {
        String * self = (String *) malloc(sizeof(String));
        self->shouldMove = false;
        self->isWrapper = false;
        self->data = NULL;
        self->set(value);
        
        return self;
    }

    static inline String * create(String &value) {
        return value.copy();
    }

    static inline double strToNum(const wchar_t * str, uint16_t len) {
        if (len > 326) return 0;  // > -MAX_DBL len
        wchar_t tmp;
        for (uint16_t i = 0; i < len; i++) {
            tmp = str[i];
            if (!iswdigit(tmp) && tmp != L'.' && tmp != L'-' && tmp != L'+') return 0;
        }
        return wcstod(str, NULL);
    }

    /*Create an empty string*/
    String():
        length(0),
        data((wchar_t *)emptyString),
        size(1 << 2),
        shouldMove(true),
        isWrapper(true)
    {}

    /*Create a string repeating the symbol from `sym`*/
    String(wchar_t sym, uint64_t _length, bool _shouldMove = false, bool _isWrapper = false):
        length(_length),
        data((wchar_t *)malloc((_length + 1) << 2)),
        size((_length + 1) << 2),
        shouldMove(_shouldMove),
        isWrapper(_isWrapper)
    {
        if (_length == 1)
            data[0] = sym;
        else
            while(_length > 0) data[--_length] = sym;

        data[length] = L'\0';
        shouldMove = _shouldMove;
    }

    /*Wrap and existing string*/
    String(uint64_t _length, wchar_t * _data, bool _shouldMove = false, bool _isWrapper = false):
        length(_length),
        data(_data),
        size((_length+1) << 2),
        shouldMove(_shouldMove),
        isWrapper(_isWrapper)
    {}

    /*For some edgy cases*/
    String(String &origin) {
        if (origin.shouldMove)
            origin.moveTo(*this);
        else
            set(origin);
    }

    void moveTo(String &destination) {
        if (!destination.isWrapper && destination.data) free(destination.data);

        destination.length = length;
        destination.size = size;
        destination.data = data;
        destination.shouldMove = shouldMove;
        destination.isWrapper = isWrapper;

        data = NULL;
    }

    String * copy() {
        String * copy = (String *) malloc(sizeof(String));
        copy->shouldMove = false;
        copy->isWrapper = false;
        copy->data = NULL;
        copy->set(*this);

        return copy;
    }

    void set(String &origin) {
        if (origin.shouldMove && !origin.isWrapper) {
            origin.moveTo(*this);
            return;
        }

        length = origin.length;
        size = origin.size;
        data = (wchar_t *) realloc(data, origin.size);

        if (data)
            memcpy(data, origin.data, origin.size);
    }

    void set(const wchar_t * value) {
        length = wcslen(value);
        size = (length + 1) << 2;
        data = (wchar_t *) realloc(data, size);

        if (data)
            memcpy(data, value, size);
    }

    void clean() {
        if (data && !isWrapper)
            free(data);
    }

    operator const wchar_t *() const {
        return data;
    }

    operator double() const {
        return strToNum(data, length);
    }

    // inline bool

    ~String() {
        clean();
    }
};

const wchar_t * String::emptyString = L"";


#endif