#ifndef CSCRATCH_STRING_H
#define CSCRATCH_STRING_H

#include <memory>
#include <cstring>
#include <cstdint>


class String {
public:
    uint32_t length = 0;  // number of chars
    uint32_t size = 0;  // memory taken in bytes
    wchar_t * data = NULL;
    bool shouldMove = false;

    static String * create(const wchar_t * value) {
        String * self = (String *) malloc(sizeof(String));
        self->length = wcslen(value);
        self->size = (self->length + 1) << 2;

        self->data = (wchar_t *) malloc(self->size);
        memcpy(self->data, value, self->size);

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

    String(bool _shouldMove = false):
        length(0),
        data((wchar_t *)malloc(1 << 2)),
        size(1 << 2)
    {
        data[0] = L'\0';
        shouldMove = _shouldMove;
    }

    String(wchar_t sym, int32_t _length, bool _shouldMove = false):
        length(_length),
        data((wchar_t *)malloc((_length + 1) << 2)),
        size((_length + 1) << 2)
    {
        for (int32_t i = 0; i < _length; i++)
            data[i] = sym;
        data[_length] = L'\0';
        shouldMove = _shouldMove;
    }

    String(uint32_t _length, wchar_t * _data, bool _shouldMove = false):
        length(_length),
        data(_data),
        size((_length+1) << 2),
        shouldMove(_shouldMove) {}

    String(String &origin) {
        if (origin.shouldMove) {
            origin.move(*this);
        } else {
            set(origin);
        }
    }

    void move(String &destination) {
        destination.length = length;
        destination.size = size;
        destination.data = data;
        destination.shouldMove = shouldMove;

        data = NULL;
        length = 0;
        size = 0;
    }

    String * copy() {
        String * copy = (String *) malloc(sizeof(String));
        
        if (shouldMove) {
            move(*copy);
            copy->shouldMove = false;
        } else {
            copy->length = length;
            copy->size = size;
            copy->data = (wchar_t *) malloc(size);
            copy->shouldMove = false;
            memcpy(copy->data, data, size);
        }

        return copy;
    }

    void set(String &origin) {
        if (origin.shouldMove) {
            origin.move(*this);
            return;
        }
        length = origin.length;
        size = origin.size;
        data = (wchar_t *) realloc(data, origin.size);

        if (data)
            memcpy(data, origin.data, origin.size);
    }

    void set(const wchar_t * value) {
        length = wcslen(data);
        size = (length + 1) << 2;
        data = (wchar_t *) realloc(data, size);

        if (data)
            memcpy(data, value, size);
    }

    void clean() {
        if (data)
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


#endif