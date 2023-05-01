#ifndef VALUE_H
#define VALUE_H

#include <cstdint>
#include <cwchar>
#include <cstring>
#include <memory>
#include <type_traits>

#include "config.h"
#include "string.h"


#define make_bool_op(op) \
    bool operator op(double value) const { \
        return number op value; \
    } \
    bool operator op(int value) const { \
        return number op value; \
    } \
    bool operator op(Value &value) { \
        return value.type == Type::NUMBER \
            ? number op value.number \
            : wcscmp(type == Type::NUMBER ? getNumberStr() : string->data, value.string->data) op 0; \
    } \
    template<typename T> \
    bool operator op(T * value) requires(std::is_same_v<T, const wchar_t>) { \
        return wcscmp(type == Type::NUMBER ? getNumberStr() : string->data, value) op 0; \
    } \
    bool operator op(String &&value) { \
        return wcscmp(type == Type::NUMBER ? getNumberStr() : string->data, value.data) op 0; \
    }


#define make_math_bin_op(op) \
    double operator op(double value) const { \
        return number op value; \
    } \
    double operator op(int value) const { \
        return number op value; \
    } \
    double operator op(Value &value) const { \
        return number op value.number; \
    } \
    template<typename T> \
    double operator op(T * value) const requires(std::is_same_v<T, const wchar_t>) { \
        return number op String::strToNum(value, wcslen(value)); \
    }


class Value {
    static wchar_t globalNumStrTmp[];
public:
    enum Type {
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
    static Value * __restrict__ create(Tv &&value) {
        Value * self = (Value*) malloc(sizeof(Value));
        
        self->previousNumber = 0;
        self->string = NULL;
        self->numberStrTmp = NULL;
        self->numberStrSize = 0;

        *self = value;
        return self;
    }

    Value() {}
    Value(double nValue, const wchar_t * sValue): number(nValue), string(String::create(sValue)) {}
    Value(double value): number(value) {}
    Value(int value): number(value) {}
    Value(const wchar_t * value): string(String::create(value)), type(Type::STRING) {
        number = (double)*string;
    }
    Value(String &&value): string(value.copy()), type(Type::STRING) {}
    Value(const Value &origin): number(origin.number), type(origin.type) {
        if (origin.type == Type::STRING)
            string = origin.string->copy();
    }

    Value * copy() const {
        Value * copy = (Value*) malloc(sizeof(Value));

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

    wchar_t * getNumberStr() {
        if (number == previousNumber && numberStrTmp) return numberStrTmp;
        previousNumber = number;

        uint16_t size = swprintf(globalNumStrTmp, 325, L"%.*f", NUM_TO_STRING_FRACTION_DIGITS, number);
        do {
            size--;
        } while(globalNumStrTmp[size] == L'0');
        if (globalNumStrTmp[size] == L'.') globalNumStrTmp[size] = L'\0';
        else globalNumStrTmp[++size] = L'\0';

        size++;

        if (size > numberStrSize) {
            numberStrTmp = (wchar_t *)realloc(numberStrTmp, size << 2);
            numberStrSize = size;
        }
        memcpy(numberStrTmp, globalNumStrTmp, size << 2);

        return numberStrTmp;
    }

    const wchar_t * toString() {
        return type == Type::STRING ? string->data : getNumberStr();
    }

    Value &operator=(const Value &origin) {
        if (origin.type == Type::NUMBER) {
            number = origin.number;
            return *this;
        }

        number = origin.number;
        string = origin.string->copy();
        type = origin.type;
        
        return *this;
    }

    Value &operator=(const wchar_t * value) {
        if (string)
            string->set(value);
        else
            string = String::create(value);

        number = (double)*string;
        type = Type::STRING;

        return *this;
    }

    Value &operator=(String &&value) {
        return operator=(value);
    }

    Value &operator=(const String &value) {
        if (string)
            string->set(value);
        else
            string = String::create(value);

        number = (double)value;
        type = Type::STRING;

        return *this;
    }

    Value &operator=(double value) {
        number = value;
        type = Type::NUMBER;
        return *this;
    }

    Value &operator=(int value) {
        number = value;
        type = Type::NUMBER;
        return *this;
    }

    Value &operator++(int) {
        number++;
        type = Type::NUMBER;
        return *this;
    }

    Value &operator--(int) {
        number--;
        type = Type::NUMBER;
        return *this;
    }

    Value &operator+=(double value) {
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

    operator double() const {
        return number;
    }

    operator const wchar_t *() {
        return type == Type::STRING ? string->data : getNumberStr();
    }

    void clean() {
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

    ~Value() {
        clean();
    }
};


#endif
