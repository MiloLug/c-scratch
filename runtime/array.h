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

#ifndef ARRAY_MAX_FULL_CLEANS_NUMBER
    #define ARRAY_MAX_FULL_CLEANS_NUMBER 8
#endif


class Array {
protected:
    int64_t capacity = ARRAY_INITIAL_SIZE + 1;  // how much it can hold
    Var * restrict__ * restrict__ data = NULL;
    Var nullValue = {0, new String(), Const::Type::STRING};
    int8_t cleansNumber = 0;

public:
    int64_t length = 0;  // how much it actually holds

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

        for (int i = 1; i < capacity; i++) {
            data[i] = new Var();
        }
    }

    template<std::size_t N>
    Array(const Const::ConstInitData (&values)[N]) {
        if (N == 0) return;
        capacity = MAX(N + 1, capacity);
        data = (Var **)malloc(capacity * sizeof(Var *));
        data[0] = &nullValue;
        length = N;

        int i = 1;
        for (; i <= N; i++) {
            data[i] = new Var(values[i - 1]);
        }
        for (; i < capacity; i++) {
            data[i] = new Var();
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
        if (length != capacity - 1) {
            *data[++length] = value;
            return;
        }

        auto old = capacity;
        capacity = (float)ARRAY_AHEAD_ALLOCATION_MULTIPLIER * capacity;
        data = (Var **)realloc((void *)data, capacity * sizeof(Var *));
        
        for (int i = old; i < capacity; i++) {
            data[i] = new Var();
        }

        *data[++length] = value;
    }

    void pop() {
        if (length) length--;
    }

    void remove(int64_t i) {
        if (i == length) return pop();
        if (i < length && i > 0) {
            auto tmp = data[i];
            for (uint64_t j = i; j < length; j++) {
                data[j] = data[j + 1];
            }
            data[length] = tmp;
            length--;
        }
    }

    void insert(int64_t i, auto && value) {
        if (i == length + 1) return push(value);
        if (i < length && i > 0) {
            if (length == capacity - 1) {
                auto old = capacity;
                capacity = (float)ARRAY_AHEAD_ALLOCATION_MULTIPLIER * capacity;
                data = (Var **)realloc((void *)data, capacity * sizeof(Var *));
                
                for (int i = old; i < capacity; i++) {
                    data[i] = new Var();
                }
            }

            length++;
            for (uint64_t j = length; j > i; j--) {
                data[j] = data[j - 1];
            }

            data[i] = value;
        }
    }

    constexpr double indexOf(auto && value) {
        for (uint64_t i = 1; i <= length; i++) {
            if (*data[i] == value) return i;
        }
        return 0;
    }

    constexpr bool contains(auto && value) { return indexOf(value) != 0; }

    constexpr force_inline__ void set(const int64_t i, auto && value) {
        if (i <= length && i > 0) *data[i] = value;
    }

    constexpr force_inline__ const Var & get(const int64_t i) const { return (i <= length && i > 0) ? *data[i] : nullValue; }

    void clean() {
        if constexpr (ARRAY_MAX_FULL_CLEANS_NUMBER > -1) {
            if (cleansNumber >= ARRAY_MAX_FULL_CLEANS_NUMBER) {
                // in this case, it's an often cleaned array, so it's better to just reuse the memory
                length = 0;
                return;
            }
        }

        cleansNumber++;
        
        if (data) {
            for (uint64_t i = 1; i < capacity; i++) {
                delete data[i];
            }

            free((void *)data);
        }

        capacity = ARRAY_INITIAL_SIZE + 1;
        length = 0;

        data = (Var **)malloc(capacity * sizeof(Var *));
        data[0] = &nullValue;
        
        for (int i = 1; i < capacity; i++) {
            data[i] = new Var();
        }
    }

    operator String() {
        if (length == 0) return String();

        uint64_t strLen = 0;
        uint64_t offset = 0;
        wchar_t *restrict__ str, tmpStr;

        // accumulate the resulting string's length
        for (uint64_t i = 1; i <= length; i++) {
            auto & tmp = data[i];
            strLen +=
                tmp->type == Const::Type::STRING ? tmp->string->length : wcslen(tmp->toString());
        }
        strLen += length;  // for the spaces between items

        str = (wchar_t *)malloc(strLen << CHAR_SIZE_POWER);

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

    ~Array() {
        if (!data) return;

        for (uint64_t i = 1; i < capacity; i++) {
            delete data[i];
        }

        free((void *)data);
        data = nullptr;
    }
};


#endif
