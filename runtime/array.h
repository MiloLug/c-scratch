#ifndef ARRAY_H
#define ARRAY_H

#include "files.h"
#include "utils.h"
#include "value.h"
#include "string/string_utils.h"

#include <filesystem>


#ifndef ARRAY_AHEAD_ALLOCATION_MULTIPLIER
    #define ARRAY_AHEAD_ALLOCATION_MULTIPLIER 1.5
#endif

#ifndef ARRAY_INITIAL_SIZE
    #define ARRAY_INITIAL_SIZE 4
#endif


class ValueArray {
protected:
    uint64_t capacity = ARRAY_INITIAL_SIZE + 1;  // how much it can hold
    Value * restrict__ * restrict__ data = NULL;
    Value nullValue = Value::ValueInitData{0, L""};

public:
    uint64_t length = 0;  // how much it actually holds
    
    static ValueArray fromFile(const std::filesystem::path & path) {
        ValueArray arr;

        File file(path, "r");

        if (!file.opened) {
            wprintf(L"Error: no data file '%ls'\n", path.wstring().c_str());
            return arr;
        }

        wchar_t * tmpLine = nullptr;
        int64_t lineLen = 0;
        bool isNumber;
        uint8_t numberBase;

        while ((lineLen = file.getwline(&tmpLine)) != -1) {
            if (lineLen == 0) {
                arr.push(String());
            } else {
                Value::storage_number_t num = strToNum(tmpLine, lineLen, &isNumber, &numberBase, true);
                if (isNumber && numberBase == 10) {
                    arr.push(num);
                } else {
                    arr.push(Value::ValueInitData{num, tmpLine});
                }
            }
        }

        return arr;
    }

    ValueArray() {
        data = (Value **)malloc(capacity * sizeof(Value *));
        data[0] = &nullValue;
    }

    template<std::size_t N>
    ValueArray(const Value::ValueInitData (&values)[N]) {
        if (N == 0) return;
        capacity = MAX(N + 1, capacity);
        data = (Value **)malloc(capacity * sizeof(Value *));
        data[0] = &nullValue;
        length = N;

        for (int i = 0; i < N; i++) {
            data[i + 1] = new Value(values[i]);
        }
    }

    ValueArray(ValueArray && origin) {
        data = origin.data;
        capacity = origin.capacity;
        length = origin.length;

        if (data) data[0] = &nullValue;

        origin.data = nullptr;
    }

    void push(auto && value) {
        if (data == NULL) {
            data = (Value **)malloc(capacity * sizeof(Value *));
            data[0] = &nullValue;
        } else if (length == capacity - 1) {
            capacity = (float)ARRAY_AHEAD_ALLOCATION_MULTIPLIER * capacity;
            data = (Value **)realloc((void *)data, capacity * sizeof(Value *));
        }

        data[length + 1] = new Value(value);
        length++;
    }

    void pop() {
        if (length) delete data[length-- + 1];
    }

    void remove(int64_t i) {
        if (i == length) return pop();
        if (i < length && i > 0) {
            delete data[i];
            for (uint64_t j = i; j < length; j++) {
                data[j] = data[j + 1];
            }
            length--;
        }
    }

    void insert(int64_t i, auto && value) {
        if (i == length + 1) {
            push(value);
        } else if (i <= length && i > 0) {
            if (this->length == this->capacity - 1) {
                capacity = (float)ARRAY_AHEAD_ALLOCATION_MULTIPLIER * capacity;
                data = (Value **)realloc((void *)data, capacity * sizeof(Value *));
            }
            for (uint64_t j = length; j >= i; j--) {
                data[j + 1] = data[j];
            }

            data[i] = new Value(value);
            length++;
        }
    }

    constexpr double indexOf(auto && value) {
        for (uint64_t i = 1; i <= length; i++) {
            if (data[i]->operator==(value)) return i;
        }
        return 0;
    }

    constexpr bool contains(auto && value) {
        return indexOf(value) != 0;
    }

    constexpr void set(const uint64_t i, auto && value) {
        if (i <= length && i > 0) data[i]->operator=(value);
    }

    constexpr Value & get(const uint64_t i) { return (i <= length) ? *data[i] : nullValue; }

    void clean() {
        if (data) {
            for (uint64_t i = 1; i <= this->length; i++) {
                delete data[i];
            }

            free((void *)data);
            data = NULL;
        }

        capacity = ARRAY_INITIAL_SIZE + 1;
        length = 0;
    }

    operator String() {
        if (length == 0) return String();

        uint64_t strLen = 0;
        uint64_t offset = 0;
        wchar_t *restrict__ str, tmpStr;

        for (uint64_t i = 1; i <= length; i++) {
            auto & tmp = data[i];
            strLen += tmp->string ? tmp->string->length : wcslen(tmp->toString());
        }
        strLen += length;  // for the spaces between items

        str = (wchar_t *)malloc(strLen << 2);

        for (uint64_t i = 1; i <= length; i++) {
            auto tmpStr = data[i]->toString();
            while (*tmpStr != L'\0') {
                str[offset++] = *(tmpStr++);
            }
            str[offset++] = L' ';
        }
        str[--offset] = L'\0';

        return String(offset, str);
    }

    ~ValueArray() { clean(); }
};


#endif
