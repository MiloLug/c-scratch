#ifndef CSCRATCH_VALUE_H
#define CSCRATCH_VALUE_H

#include "config.h"
#include "math/number_type.h"
#include "string/string.h"
#include "utils.h"

#include <cstdint>
#include <cwchar>
#include <memory>
#include <type_traits>


#define make_bool_op(op)                                                                           \
    bool operator op(Value & value) {                                                              \
        return value.type == Type::NUMBER                                                          \
            ? type == Type::NUMBER ? number op value.number                                        \
                                   : wcscmp(string->data, value.getNumberStr()) op 0               \
            : wcscmp(type == Type::NUMBER ? getNumberStr() : string->data, value.string->data)     \
                op 0;                                                                              \
    }                                                                                              \
    bool operator op(OneOfT<const wchar_t> auto * restrict__ value) {                              \
        if (type == Type::STRING) {                                                                \
            return wcscmp(string->data, value) op 0;                                               \
        }                                                                                          \
        getNumberStr();                                                                            \
        return wcscmp(numberStrTmp, value);                                                        \
    }                                                                                              \
    bool operator op(const String & value) {                                                       \
        return wcscmp(type == Type::NUMBER ? getNumberStr() : string->data, value.data) op 0;      \
    }                                                                                              \
    constexpr bool operator op(NumberT auto value) const { return number op value; }


#define make_math_bin_op(op)                                                                       \
    constexpr storage_number_t operator op(NumberT auto value) const { return number op value; }   \
    constexpr storage_number_t operator op(Value & value) const { return number op value.number; } \
    storage_number_t operator op(OneOfT<const wchar_t> auto * restrict__ value) const {            \
        return number op strToNum(value, wcslen(value));                                           \
    }


class Value {
protected:
    static wchar_t globalNumStrTmp[];
    static constexpr wchar_t infinityStr[] = L"Infinity";
    static constexpr wchar_t negInfinityStr[] = L"-Infinity";

public:
    enum Type : uint8_t { STRING, NUMBER };

    struct ValueInitData {
        const wchar_t * str = nullptr;
        storage_number_t number = 0;
        Type type = Type::NUMBER;

        constexpr ValueInitData(NumberT auto _number): number{(storage_number_t)_number} {}

        ValueInitData(OneOfT<const wchar_t> auto * restrict__ _str):
            str{_str},
            number{(storage_number_t)strToNum(str, wcslen(str))},
            type{Type::STRING} {}

        constexpr ValueInitData(double _number, const wchar_t * _str):
            str{_str},
            number{(storage_number_t)_number},
            type{Type::STRING} {}
    };

    Type type = Type::NUMBER;

    storage_number_t number = 0;
    storage_number_t previousNumber = 0;  // for "number to string" caching

    String * string = NULL;
    
    wchar_t * numberStrTmp = NULL;
    uint16_t numberStrSize = 0;
    uint16_t numberStrLen = 0;

    constexpr Value() {}

    Value(const ValueInitData & data):
        number{data.number},
        type{data.type},
        string{data.str ? new String(data.str) : nullptr} {}

    constexpr Value(NumberT auto value): number{(storage_number_t)value} {}

    Value(OneOfT<const wchar_t> auto * restrict__ value):
        string{new String(value)},
        type{Type::STRING} {
        number = (double)*string;
    }
    Value(String && value): string{new String((String &&)value)}, type{Type::STRING} {
        number = (double)*string;
    }
    Value(const String & value): string{new String(value)}, type{Type::STRING} {
        number = (double)*string;
    }

    Value(const Value & origin): number{origin.number}, type{origin.type} {
        if (origin.type == Type::STRING) string = new String(*origin.string);
    }

    Value & operator=(const Value & origin) {
        number = origin.number;
        type = origin.type;

        if (type == Type::NUMBER) {
            return *this;
        }

        if (string)
            *string = *origin.string;
        else
            string = new String(*origin.string);

        return *this;
    }

    Value & operator=(const ValueInitData & data) {
        number = data.number;
        type = data.type;

        if (data.str) {
            if (string)
                *string = data.str;
            else
                string = new String(data.str);
        }

        return *this;
    }

    Value & operator=(OneOfT<const wchar_t> auto * restrict__ value) {
        if (string)
            *string = value;
        else
            string = new String(value);

        number = (double)*string;
        type = Type::STRING;

        return *this;
    }

    Value & operator=(const String & value) {
        if (string)
            *string = value;
        else
            string = new String(value);

        number = (double)value;
        type = Type::STRING;

        return *this;
    }
    Value & operator=(String && value) {
        if (string)
            *string = (String &&)value;
        else
            string = new String((String &&)value);

        number = (double)value;
        type = Type::STRING;

        return *this;
    }

    Value & operator=(NumberT auto value) {
        number = value;
        type = Type::NUMBER;
        return *this;
    }

    Value & operator++(int) {
        number++;
        type = Type::NUMBER;
        return *this;
    }

    Value & operator--(int) {
        number--;
        type = Type::NUMBER;
        return *this;
    }

    Value & operator+=(double value) {
        number += value;
        type = Type::NUMBER;
        return *this;
    }

    make_bool_op(<=);
    make_bool_op(>=);
    make_bool_op(<);
    make_bool_op(>);
    make_bool_op(==);

    make_math_bin_op(-);
    make_math_bin_op(+);
    make_math_bin_op(/);
    make_math_bin_op(*);

    bool operator!() { return !number; }
    constexpr operator storage_number_t() { return number; }

    operator const wchar_t *() { return type == Type::STRING ? string->data : getNumberStr(); }

    wchar_t * restrict__ & getNumberStr() {
        constexpr uint16_t fracBaseLen = 15;
        constexpr double minExponential = 1e+21;

        if (number == previousNumber && numberStrTmp) return numberStrTmp;
        previousNumber = number;

        if (number == NT_INF || number == NT_NEG_INF) {
            if (numberStrSize < sizeof(negInfinityStr)) {
                numberStrTmp = (wchar_t *)realloc((void *)numberStrTmp, sizeof(negInfinityStr));
                numberStrSize = sizeof(negInfinityStr);
                numberStrLen = (numberStrSize >> 2) - 1;
            }
            wcscpy(numberStrTmp, number == NT_INF ? infinityStr : negInfinityStr);
            if (numberStrTmp[0] != L'-') numberStrLen--;
            return numberStrTmp;
        }

        bool useExpNotation = std::abs(number) >= minExponential;
        uint16_t size = useExpNotation ? swprintf(globalNumStrTmp, 325, L"%.15e", number)
                                       : swprintf(globalNumStrTmp, 325, L"%.15f", number);

        // it's important to try this branch first to avoid jumps, sine no-exponential numbers are more common
        if (!useExpNotation) {
            do {
                size--;
            } while (globalNumStrTmp[size] == L'0');
            if (globalNumStrTmp[size] == L'.')
                globalNumStrTmp[size] = L'\0';
            else
                globalNumStrTmp[++size] = L'\0';

            size++;

            if ((size << 2) > numberStrSize) {
                numberStrSize = size << 2;
                numberStrTmp = (wchar_t *)realloc((void *)numberStrTmp, numberStrSize);
            }
            memcpy(numberStrTmp, globalNumStrTmp, size << 2);
            numberStrLen = size - 1;
            return numberStrTmp;
        }

        uint16_t intEnd;
        uint16_t fracEnd = 0;
        uint16_t expPartStart;
        uint16_t i = -1;

        while (globalNumStrTmp[++i] != L'.')
            ;
        intEnd = i;

        while (globalNumStrTmp[++i] != L'e') {
            if (globalNumStrTmp[i] != L'0') fracEnd = i + 1;
        }

        expPartStart = globalNumStrTmp[i + 1] != L'0' ? i : 0;

        size = (intEnd + size - expPartStart + (fracEnd == 0 ? 0 : fracEnd - intEnd)) + 1;
        if ((size << 2) > numberStrSize) {
            numberStrSize = size << 2;
            numberStrTmp = (wchar_t *)realloc((void *)numberStrTmp, numberStrSize);
        }
        numberStrTmp[size] = L'\0';

        if (fracEnd != 0) {
            memcpy(numberStrTmp, globalNumStrTmp, fracEnd << 2);
            i = fracEnd;
        } else {
            memcpy(numberStrTmp, globalNumStrTmp, intEnd << 2);
            i = intEnd;
        }

        memcpy(numberStrTmp + i, globalNumStrTmp + expPartStart, (size - expPartStart) << 2);
        numberStrLen = size - 1;

        return numberStrTmp;
    }

    const wchar_t * restrict__ toString() {
        return type == Type::STRING ? string->data : getNumberStr();
    }

    constexpr void clean() {
        if (numberStrTmp) {
            free(numberStrTmp);
            numberStrTmp = NULL;
            numberStrSize = 0;
            numberStrLen = 0;
        }

        if (string) {
            string->clean();
            free(string);
            string = NULL;
        }
    }

    constexpr ~Value() { clean(); }
};


#endif
