#ifndef VALUE_H
#define VALUE_H

#include <cstdint>
#include <cwchar>
#include <string>
#include <memory>
#include <type_traits>

#include "utils.h"
#include "config.h"
#include "string.h"


#define make_bool_op(op) \
    constexpr bool operator op(Value &value) { \
        return value.type == Type::NUMBER \
            ? type == Type::NUMBER \
                ? number op value.number \
                : std::char_traits<wchar_t>::compare(string->data, value.getNumberStr(), string->length) op 0 \
            : std::char_traits<wchar_t>::compare( \
                type == Type::NUMBER \
                    ? getNumberStr() \
                    : string->data, \
                value.string->data, \
                value.string->length \
            ) op 0; \
    } \
    template<typename T> \
    constexpr bool operator op(T * value) requires(std::is_same_v<T, const wchar_t>) { \
        if (type == Type::STRING) { \
            return std::char_traits<wchar_t>::compare(string->data, value, string->length) op 0; \
        } \
        getNumberStr(); \
        return std::char_traits<wchar_t>::compare(numberStrTmp, value, numberStrSize-1); \
    } \
    constexpr bool operator op(String &&value) { \
        return std::char_traits<wchar_t>::compare(type == Type::NUMBER ? getNumberStr() : string->data, value.data, value.length) op 0; \
    } \
    constexpr bool operator op(double value) const { \
        return number op value; \
    } \
    constexpr bool operator op(float value) const { \
        return number op value; \
    } \
    constexpr bool operator op(int value) const { \
        return number op value; \
    }


#define make_math_bin_op(op) \
    constexpr storage_number_t operator op(double value) const { \
        return number op value; \
    } \
    constexpr storage_number_t operator op(float value) const { \
        return number op value; \
    } \
    constexpr storage_number_t operator op(int value) const { \
        return number op value; \
    } \
    constexpr storage_number_t operator op(Value &value) const { \
        return number op value.number; \
    } \
    template<typename T> \
    constexpr storage_number_t operator op(T * value) const requires(std::is_same_v<T, const wchar_t>) { \
        return number op String::strToNum(value, std::char_traits<wchar_t>::length(value)); \
    }


class Value {
protected:
    static wchar_t globalNumStrTmp[];
    
public:
    enum Type : uint8_t {
        STRING,
        NUMBER
    };

    #ifdef USE_VALUE_FLOAT
        typedef float storage_number_t;
    #else
        typedef double storage_number_t;
    #endif

    Type type = Type::NUMBER;

    storage_number_t number = 0;
    storage_number_t previousNumber = 0;  // for "number to string" caching

    String * string = NULL;
    wchar_t * numberStrTmp = NULL;
    uint16_t numberStrSize = 0;

    template<typename Tv>
    constexpr static Value * restrict__ create(Tv &&value) {
        Value * restrict__ self = (Value*) malloc(sizeof(Value));
        
        self->previousNumber = 0;
        self->string = NULL;
        self->numberStrTmp = NULL;
        self->numberStrSize = 0;

        *self = value;
        return self;
    }

    constexpr Value() {}
    constexpr Value(double nValue, const wchar_t * restrict__ sValue): number(nValue), string(String::create(sValue)) {}
    constexpr Value(double value): number(value) {}
    constexpr Value(int value): number(value) {}
    constexpr Value(const wchar_t * restrict__ value): string(String::create(value)), type(Type::STRING) {
        number = (double)*string;
    }
    constexpr Value(String &&value): string(value.copy()), type(Type::STRING) {}
    constexpr Value(const Value &origin): number(origin.number), type(origin.type) {
        if (origin.type == Type::STRING)
            string = origin.string->copy();
    }

    constexpr Value * restrict__ copy() const {
        Value * restrict__ copy = (Value*) malloc(sizeof(Value));

        copy->previousNumber = 0;
        copy->string = NULL;
        copy->numberStrTmp = NULL;
        copy->numberStrSize = 0;
        copy->type = type;
        copy->number = number;

        // speed up numeric operations a little by avoiding jumps in case of NUMBER type
        if (type == Type::NUMBER) return copy;

        copy->string = string->copy();
        return copy;
    }

    constexpr wchar_t * restrict__ &getNumberStr() {
        constexpr uint16_t fracBaseLen = 15;
        constexpr double minExponential = 1e+21;

        if (number == previousNumber && numberStrTmp) return numberStrTmp;
        previousNumber = number;

        bool useExpNotation = abs(number) >= minExponential;
        uint16_t size = useExpNotation ? swprintf(globalNumStrTmp, 325, L"%.15e", number) : swprintf(globalNumStrTmp, 325, L"%.15f", number);

        // it's important to try this branch first to avoid jumps, sine no-exponential numbers are more common
        if (!useExpNotation) {
            do {
                size--;
            } while(globalNumStrTmp[size] == L'0');
            if (globalNumStrTmp[size] == L'.') globalNumStrTmp[size] = L'\0';
            else globalNumStrTmp[++size] = L'\0';

            size++;

            if (size > numberStrSize) {
                numberStrTmp = (wchar_t *)realloc((void *)numberStrTmp, size << 2);
                numberStrSize = size;
            }
            memcpy(numberStrTmp, globalNumStrTmp, size << 2);

            return numberStrTmp;
        }

        uint16_t intEnd;
        uint16_t fracEnd = 0;
        uint16_t expPartStart;
        uint16_t i = -1;
        
        while(globalNumStrTmp[++i] != L'.');
        intEnd = i;

        while(globalNumStrTmp[++i] != L'e') {
            if (globalNumStrTmp[i] != L'0') fracEnd = i + 1;
        }

        expPartStart = globalNumStrTmp[i + 1] != L'0' ? i : 0;  

        size = (
            intEnd
            + size - expPartStart
            + (fracEnd == 0 ? 0 : fracEnd - intEnd)
        ) + 1;
        if (size > numberStrSize) {
            numberStrTmp = (wchar_t *)realloc((void *)numberStrTmp, size << 2);
            numberStrSize = size;
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

        return numberStrTmp;
    }

    constexpr const wchar_t * toString() {
        return type == Type::STRING ? string->data : getNumberStr();
    }

    constexpr Value &operator=(const Value &origin) {
        if (origin.type == Type::NUMBER) {
            number = origin.number;
            return *this;
        }

        number = origin.number;
        if (string)
            string->set(*origin.string);
        else
            string = origin.string->copy();
        type = Type::STRING;
        
        return *this;
    }

    constexpr Value &operator=(const wchar_t * restrict__ value) {
        if (string)
            string->set(value);
        else
            string = String::create(value);

        number = (double)*string;
        type = Type::STRING;

        return *this;
    }

    constexpr Value &operator=(String &&value) {
        return operator=(value);
    }

    constexpr Value &operator=(const String &value) {
        if (string)
            string->set(value);
        else
            string = String::create(value);

        number = (double)value;
        type = Type::STRING;

        return *this;
    }

    constexpr Value &operator=(double value) {
        number = value;
        type = Type::NUMBER;
        return *this;
    }

    constexpr Value &operator=(int value) {
        number = value;
        type = Type::NUMBER;
        return *this;
    }

    constexpr Value &operator++(int) {
        number++;
        type = Type::NUMBER;
        return *this;
    }

    constexpr Value &operator--(int) {
        number--;
        type = Type::NUMBER;
        return *this;
    }

    constexpr Value &operator+=(double value) {
        number += value;
        type = Type::NUMBER;
        return *this;
    }

    make_bool_op(<=)
    make_bool_op(>=)
    make_bool_op(<)
    make_bool_op(>)
    make_bool_op(==)

    make_math_bin_op(-)
    make_math_bin_op(+)
    make_math_bin_op(/)
    make_math_bin_op(*)

    constexpr bool operator !() {
        return !number;
    }

    constexpr operator storage_number_t() {
        return number;
    }

    constexpr operator const wchar_t *() {
        return type == Type::STRING ? string->data : getNumberStr();
    }

    constexpr void clean() {
        if (string) {
            string->clean();
            free(string);
        }
        if (numberStrTmp) {
            free(numberStrTmp);
        }

        string = NULL;
        numberStrTmp = NULL;
        numberStrSize = 0;
    }

    constexpr ~Value() {
        clean();
    }
};


#endif
