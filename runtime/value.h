#ifndef VALUE_H
#define VALUE_H

#include <cstdint>
#include <cwchar>
#include <cstring>
#include <memory>

#include "string.h"


#define make_bool_op(op) \
    inline bool operator op(double value) { \
        return (string == NULL ? number : getStrNumber()) op value; \
    } \
    inline bool operator op(uint64_t value) { \
        return (string == NULL ? (uint64_t)number : getStrNumber()) op value; \
    } \
    inline bool operator op(int value) { \
        return (string == NULL ? (int)number : getStrNumber()) op value; \
    } \
    inline bool operator op(const wchar_t * value) { \
        return wcscmp(string == NULL ? getNumberStr() : string->data, value) op 0; \
    } \
    inline bool operator op(Value &value) { \
        return value.string ? (*this op value.string->data) : (*this op value.number); \
    }


class Value {
public:
    double number = 0;
    String * string = NULL;
    wchar_t * numberStrTmp = NULL;
    uint16_t numberStrSize = 0;
    bool updateNumberStr = true;
    bool updateStrNumber = true;

    template<typename Tv>
    static inline Value * create(Tv value) {
        Value * self = (Value*) malloc(sizeof(Value));
        
        self->string = NULL;
        self->numberStrTmp = NULL;
        self->numberStrSize = 0;
        self->updateNumberStr = true;
        self->updateStrNumber = true;

        *self = value;
        return self;
    }

    inline Value(double nValue, const wchar_t * sValue): number(nValue), string(String::create(sValue)) {}
    inline Value(double value): number(value) {}
    inline Value(int value): number(value) {}
    inline Value(const wchar_t * value): string(String::create(value)) {}
    inline Value(const Value &origin): number(origin.number), string(origin.string->copy()) {}

    inline Value * copy() const {
        Value * copy = (Value*) malloc(sizeof(Value));

        copy->number = number;
        copy->numberStrTmp = NULL;
        copy->numberStrSize = 0;
        copy->updateNumberStr = true;
        copy->updateStrNumber = true;

        if ((copy->string = string)) {
            copy->string = string->copy();
        }

        return copy;
    }

    inline wchar_t * getNumberStr() {
        if (!updateNumberStr) return numberStrTmp;
        updateNumberStr = false;

        uint16_t size = snprintf(NULL, 0, "%.*f", NUM_TO_STRING_FRACTION_DIGITS, number) + 1;
        if (size > numberStrSize) {
            numberStrTmp = (wchar_t *)realloc(numberStrTmp, sizeof(wchar_t) * size);
        }
        size = swprintf(numberStrTmp, size, L"%.*f", NUM_TO_STRING_FRACTION_DIGITS, number);

        // Cut off the leading zeroes
        uint16_t i;
        for (i = size - 1; numberStrTmp[i] != L'.' && numberStrTmp[i] == L'0'; i--);
        if (numberStrTmp[i] == L'.') numberStrTmp[i] = L'\0';
        else numberStrTmp[i+1] = L'\0';

        return numberStrTmp;
    }
    
    inline double getStrNumber() {
        if (!updateStrNumber) return number;
        updateStrNumber = false;
        wchar_t tmp;
        for (uint64_t i = 0; i < string->length; i++) {
            tmp = string->data[i];
            if (!iswdigit(tmp) && tmp != L'.') return 0;
        }
        number = wcstod(string->data, NULL);
        return number;
    }

    inline const wchar_t * toString() {
        return string ? string->data : getNumberStr();
    }

    inline Value &operator=(const Value &origin) {
        number = origin.number;
        updateNumberStr = true;
        updateStrNumber = true;

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
        updateStrNumber = true;

        if (string)
            string->set(value);
        else
            string = String::create(value);

        return *this;
    }

    inline Value &operator=(double value) {
        number = value;
        updateNumberStr = true;

        if (string) {
            string->clean();
            free(string);
            string = NULL;
        }

        return *this;
    }

    inline Value &operator=(int value) {
        return this->operator=(static_cast<double>(value));
    }

    inline Value &operator++(int) {
        if (string) {
            getStrNumber();
            string->clean();
            free(string);
            string = NULL;
        }

        number++;
        updateNumberStr = true;
        return *this;
    }

    inline Value &operator+=(double value) {
        if (string) {
            getStrNumber();
            string->clean();
            free(string);
            string = NULL;
        }

        number += value;
        updateNumberStr = true;
        return *this;
    }

    make_bool_op(<=)
    make_bool_op(>=)
    make_bool_op(<)
    make_bool_op(>)
    make_bool_op(==)

    inline operator double() {
        return string ? getStrNumber() : number;
    }

    inline operator const wchar_t *() {
        return string ? string->data : getNumberStr();
    }

    inline void clean() {
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
        updateNumberStr = true;
        updateStrNumber = true;
    }

    ~Value() {
        clean();
    }
};


#endif
