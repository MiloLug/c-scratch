#ifndef ARRAY_H
#define ARRAY_H

#include "value.h"
#include "math.h"

#ifndef ARRAY_AHEAD_ALLOCATION_MULTIPLIER
    #define ARRAY_AHEAD_ALLOCATION_MULTIPLIER 1.5
#endif

#ifndef ARRAY_INITIAL_SIZE   
    #define ARRAY_INITIAL_SIZE 4
#endif


class ValueArray {
public:
    uint64_t capacity = ARRAY_INITIAL_SIZE+1;  // how much it can hold
    uint64_t length = 0;  // how much it actually holds
    Value * * data = NULL;
    Value nullValue = {0, L""};

    ValueArray() {}

    template<std::size_t N>
    ValueArray(const Value (&values)[N]) {
        if (N == 0) return;
        capacity = MAX(N+1, ARRAY_INITIAL_SIZE+1);
        data = (Value * *) malloc(capacity * sizeof(Value *));
        length = N;

        for(int i = 0; i < N; i++) {
            data[i+1] = values[i].copy();
        }
    }

    template<typename Tv>
    void push(Tv value) {
        if (data == NULL) {
            data = (Value * *) malloc(capacity * sizeof(Value *));
        }
        else if (length == capacity - 1) {
            capacity = (float)ARRAY_AHEAD_ALLOCATION_MULTIPLIER * capacity;
            data = (Value * *) realloc(data, capacity * sizeof(Value *));
        }

        data[length+1] = Value::create(value);
        length++;
    }

    inline void pop() {
        if (length) data[length-- + 1]->clean();
    }

    void remove(uint64_t i) {
        if (i <= length && i > 0) {
            data[i]->clean();
            free(data[i]);
            for (uint64_t j = i; j < length; j++) {
                data[j] = data[j+1];
            }
            length--;
        }
    }

    template<typename Tv>
    void insert(uint64_t i, Tv value) {
        if (i == length + 1) {
            push(value);
        } else if (i <= length && i > 0) {
            if (this->length == this->capacity - 1) {
                capacity = (float)ARRAY_AHEAD_ALLOCATION_MULTIPLIER * capacity;
                data = (Value * *) realloc(data, capacity * sizeof(Value *));
            }
            for (uint64_t j = length; j >= i; j--) {
                data[j+1] = data[j];
            }

            data[i] = Value::create(value);
            length++;
        }
    }

    template<typename Tv>
    inline double indexOf(Tv value) {
        for (uint64_t i = 1; i <= length; i++) {
            if (data[i]->operator==(value)) return i;
        }
        return 0;
    }

    template<typename Tv>
    inline double contains(Tv value) {
        return indexOf(value) != 0;
    }

    template<typename Tv>
    inline void set(uint64_t i, Tv value) {
        if (i <= length && i > 0) *data[i] = value;
    }

    inline Value &get(const uint64_t i) {
        return (i <= length && i > 0) ? *data[i] : nullValue;
    }

    inline void clean() {
        for (int64_t i = 1; i <= this->length; i++) {
            data[i]->clean();
            free(data[i]);
        }

        free(data);
        data = NULL;
        capacity = ARRAY_INITIAL_SIZE+1;
        length = 0;
    }

    ~ValueArray() {
        clean();
    }
};


#endif
