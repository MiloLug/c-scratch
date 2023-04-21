#ifndef VALUE_H
#define VALUE_H

#include <cstdint>
#include <cwchar>
#include <cstring>
#include <memory>


class String {
public:
    uint32_t length = 0;  // number of chars
    uint32_t size = 0;  // memory taken in bytes
    wchar_t * data = NULL;

    static String * create(const wchar_t * value) {
        String * self = (String *) malloc(sizeof(String));
        self->length = wcslen(value);
        self->size = (self->length + 1) * sizeof(wchar_t);

        self->data = (wchar_t *) malloc(self->size);
        memcpy(self->data, value, self->size);

        return self;
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
        size = (length + 1) * sizeof(wchar_t);
        data = (wchar_t *) realloc(data, size);

        if (data)
            memcpy(data, value, size);
    }

    inline void clean() {
        free(data);
    }
};


class Value {
public:
    double number = 0;
    String * string = NULL;

    template<typename Tv>
    static inline Value * create(Tv value) {
        Value * self = (Value*) malloc(sizeof(Value));
        self->string = NULL;
        *self = value;
        return self;
    }

    inline Value * copy() const {
        Value * copy = (Value*) malloc(sizeof(Value));

        copy->number = number;
        if ((copy->string = string)) {
            copy->string = string->copy();
        }

        return copy;
    }

    inline Value(double nValue, const wchar_t * sValue): number(nValue), string(String::create(sValue)) {}
    inline Value(double value): number(value) {}
    inline Value(int value): number(value) {}
    inline Value(const wchar_t * value): string(String::create(value)) {}
    inline Value(const Value &origin): number(origin.number), string(origin.string->copy()) {}

    inline Value &operator=(const Value &origin) {
        number = origin.number;

        if (string) {
            if (origin.string) {
                string->set(*origin.string);
            } else {
                string->clean();
                free(string);
                string = NULL;
            }
        } else if (origin.string) {
            string = origin.string->copy();
        }

        return *this;
    }

    inline Value &operator=(const wchar_t * value) {
        number = 0;
        if (string)
            string->set(value);
        else
            string = String::create(value);

        return *this;
    }

    inline Value &operator=(double value) {
        if (string) {
            string->clean();
            free(string);
            string = NULL;
        }
        number = value;

        return *this;
    }

    inline Value &operator=(int value) {
        return this->operator=(static_cast<double>(value));
    }

    inline operator double() const {
        return number;
    }

    inline Value &operator++(int) {
        number++;
        return *this;
    }

    inline void clean() {
        if (string) {
            string->clean();
            free(string);
            string = NULL;
        }
    }

    ~Value() {
        clean();
    }
};


#endif
