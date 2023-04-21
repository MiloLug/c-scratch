#ifndef ARRAY_H
#define ARRAY_H

#include "value.h"

#ifndef ARRAY_AHEAD_ALLOCATION_MULTIPLIER
    #define ARRAY_AHEAD_ALLOCATION_MULTIPLIER 1.5
#endif

#ifndef ARRAY_INITIAL_SIZE   
    #define ARRAY_INITIAL_SIZE 4
#endif


class ValueArray {
public:
    uint64_t capacity = ARRAY_INITIAL_SIZE;  // how much it can hold
    uint64_t length = 0;  // how much it actually holds
    Value * * data = NULL;
    Value nullValue {.number = 0, .string = String::create(L"")};

    void push(const Value &value) {
        if (data == NULL) {
            data = (Value * *) malloc(capacity * sizeof(Value *));
        }
        else if (this->length == this->capacity) {
            capacity = (float)ARRAY_AHEAD_ALLOCATION_MULTIPLIER * capacity;
            data = (Value * *) realloc(data, capacity * sizeof(Value *));
        }

        data[length] = Value::copy(value);
        length++;
    }

    inline void push(const wchar_t * value) {
        return push({.string = String::create(value)});
    }

    inline void push(const double value) {
        push({.number = value});
    }

    template<typename T>
    inline void set(const uint64_t i, T value) {
        if (i < length) data[i]->set(value);
    }

    inline Value * get(const uint64_t i) {
        return i < length ? data[i] : &nullValue;
    }

    inline void clean() {
        for (int64_t i = 0; i < this->length; i++) {
            this->data[i]->clean();
            free(this->data[i]);
        }

        free(this->data);
        this->data = NULL;
    }

    ~ValueArray() {
        clean();
    }
};


#endif
