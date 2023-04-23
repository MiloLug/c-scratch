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

    static String * create(const wchar_t * value) {
        String * self = (String *) malloc(sizeof(String));
        self->length = wcslen(value);
        self->size = (self->length + 1) << 2;

        self->data = (wchar_t *) malloc(self->size);
        memcpy(self->data, value, self->size);

        return self;
    }

    String(uint32_t _length, wchar_t * _data):
        length(_length),
        data(_data),
        size((_length+1) << 2) {}

    String(String &origin) {
        length = origin.length;
        size = origin.size;
        data = origin.data;

        origin.data = NULL;
    }

    inline String * copy() {
        String * copy = (String *) malloc(sizeof(String));
        
        copy->length = length;
        copy->size = size;
        copy->data = (wchar_t *) malloc(size);

        memcpy(copy->data, data, size);

        return copy;
    }

    inline void set(const String &origin) {
        length = origin.length;
        size = origin.size;
        data = (wchar_t *) realloc(data, origin.size);

        if (data)
            memcpy(data, origin.data, origin.size);
    }

    inline void set(const wchar_t * value) {
        length = wcslen(data);
        size = (length + 1) << 2;
        data = (wchar_t *) realloc(data, size);

        if (data)
            memcpy(data, value, size);
    }

    inline void clean() {
        if (data)
            free(data);
    }

    inline operator const wchar_t *() const {
        return data;
    }

    // inline bool

    ~String() {
        clean();
    }
};


#endif