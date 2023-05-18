#ifndef CSCRATCH_ARRAY_H
#define CSCRATCH_ARRAY_H

#include "files.h"
#include "math/number_type.h"
#include "string/string_utils.h"
#include "utils.h"
#include "value.h"

#include <filesystem>


#ifndef ARRAY_AHEAD_ALLOCATION_MULTIPLIER
    #define ARRAY_AHEAD_ALLOCATION_MULTIPLIER 1.5
#endif

#ifndef ARRAY_INITIAL_SIZE
    #define ARRAY_INITIAL_SIZE 4
#endif


class Array {
protected:
    uint64_t capacity = ARRAY_INITIAL_SIZE + 1;  // how much it can hold
    Var * restrict__ * restrict__ data = NULL;
    Var nullValue = {0, new String(), Const::Type::STRING};

public:
    uint64_t length = 0;  // how much it actually holds

    static Array fromFile(const std::filesystem::path & path) {
        Array arr;

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
                storage_number_t num = strToNum(tmpLine, lineLen, &isNumber, &numberBase, true);
                if (isNumber && numberBase == 10) {
                    arr.push(num);
                } else {
                    arr.push(Const::ConstInitData{num, tmpLine});
                }
            }
        }

        return arr;
    }

    Array() {
        data = (Var **)malloc(capacity * sizeof(Var *));
        data[0] = &nullValue;
    }

    template<std::size_t N>
    Array(const Const::ConstInitData (&values)[N]) {
        if (N == 0) return;
        capacity = MAX(N + 1, capacity);
        data = (Var **)malloc(capacity * sizeof(Var *));
        data[0] = &nullValue;
        length = N;

        for (int i = 0; i < N; i++) {
            data[i + 1] = new Var(values[i]);
        }
    }

    Array(Array && origin) {
        data = origin.data;
        capacity = origin.capacity;
        length = origin.length;

        if (data) data[0] = &nullValue;

        origin.data = nullptr;
    }

    void push(auto && value) {
        if (length == capacity - 1) {
            capacity = (float)ARRAY_AHEAD_ALLOCATION_MULTIPLIER * capacity;
            data = (Var **)realloc((void *)data, capacity * sizeof(Var *));
        }

        data[length + 1] = new Var(value);
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
                data = (Var **)realloc((void *)data, capacity * sizeof(Var *));
            }
            for (uint64_t j = length; j >= i; j--) {
                data[j + 1] = data[j];
            }

            data[i] = new Var(value);
            length++;
        }
    }

    constexpr double indexOf(auto && value) {
        for (uint64_t i = 1; i <= length; i++) {
            if (*data[i] == value) return i;
        }
        return 0;
    }

    constexpr bool contains(auto && value) { return indexOf(value) != 0; }

    constexpr void set(const uint64_t i, auto && value) {
        if (i <= length && i > 0) *data[i] = value;
    }

    constexpr const Var & get(const uint64_t i) { return (i <= length) ? *data[i] : nullValue; }

    void clean() {
        if (data) {
            for (uint64_t i = 1; i <= this->length; i++) {
                delete data[i];
            }

            free((void *)data);
        }

        capacity = ARRAY_INITIAL_SIZE + 1;
        length = 0;

        data = (Var **)malloc(capacity * sizeof(Var *));
        data[0] = &nullValue;
    }

    operator String() {
        if (length == 0) return String();

        uint64_t strLen = 0;
        uint64_t offset = 0;
        wchar_t *restrict__ str, tmpStr;

        for (uint64_t i = 1; i <= length; i++) {
            auto & tmp = data[i];
            strLen +=
                tmp->type == Const::Type::STRING ? tmp->string->length : wcslen(tmp->toString());
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

    ~Array() { clean(); }
};


#endif
