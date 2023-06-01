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
    bool operator op(const Const & value) const {                                                  \
        return value.type == Type::NUMBER                                                          \
            ? type == Type::NUMBER ? number op value.number                                        \
                                   : wcscmp(string->data, value.getNumberStr()) op 0               \
            : wcscmp(type == Type::NUMBER ? getNumberStr() : string->data, value.string->data)     \
                op 0;                                                                              \
    }                                                                                              \
    bool operator op(OneOfT<const wchar_t> auto * restrict__ value) const {                        \
        if (type == Type::STRING) {                                                                \
            return wcscmp(string->data, value) op 0;                                               \
        }                                                                                          \
        return wcscmp(getNumberStr(), value) op 0;                                                 \
    }                                                                                              \
    bool operator op(const String & value) const {                                                 \
        return wcscmp(type == Type::NUMBER ? getNumberStr() : string->data, value.data) op 0;      \
    }                                                                                              \
    constexpr bool operator op(NumberT auto value) const {                                         \
        return type == Type::NUMBER ? number op value                                              \
                                    : wcscmp(string->data, Const(value).getNumberStr()) op 0;      \
    }


#define make_math_bin_op(op)                                                                       \
    constexpr storage_number_t operator op(NumberT auto value) const { return number op value; }   \
    constexpr storage_number_t operator op(const Const & value) const {                            \
        return number op value.number;                                                             \
    }                                                                                              \
    storage_number_t operator op(OneOfT<const wchar_t> auto * restrict__ value) const {            \
        return number op strToNum(value, wcslen(value));                                           \
    }


class Const {
protected:
    static wchar_t globalNumStrTmp[];
    static constexpr wchar_t infinityStr[] = L"Infinity";
    static constexpr wchar_t negInfinityStr[] = L"-Infinity";

public:
    enum class Type : uint8_t { STRING, NUMBER };

    struct ConstInitData {
        const wchar_t * str = nullptr;
        storage_number_t number = 0;
        Type type = Type::NUMBER;

        constexpr ConstInitData(NumberT auto _number): number{(storage_number_t)_number} {}

        ConstInitData(OneOfT<const wchar_t> auto * restrict__ _str):
            str{_str},
            number{(storage_number_t)strToNum(str, wcslen(str))},
            type{Type::STRING} {}

        constexpr ConstInitData(double _number, const wchar_t * _str):
            str{_str},
            number{(storage_number_t)_number},
            type{Type::STRING} {}
    };

    Type type = Type::NUMBER;

    storage_number_t number = 0;
    mutable storage_number_t previousNumber = 0;  // for "number to string" caching

    String * string = nullptr;

    mutable wchar_t * numberStrTmp = nullptr;
    mutable uint16_t numberStrSize = 0;
    mutable uint16_t numberStrLen = 0;

    constexpr Const() {}

    constexpr force_inline__ Const(NumberT auto num, String * str, Type _type = Type::NUMBER):
        number{(storage_number_t)num},
        type{_type},
        string{str} {}

    Const(const ConstInitData & data):
        number{data.number},
        type{data.type},
        string{data.str ? new String(data.str) : nullptr} {}

    constexpr Const(NumberT auto value): number{(storage_number_t)value} {}

    Const(OneOfT<const wchar_t> auto * restrict__ value):
        string{new String(value)},
        type{Type::STRING} {
        number = (double)*string;
    }
    Const(String && value):
        number{(storage_number_t)(double)value},
        string{new String((String &&)value)},
        type{Type::STRING} {}
    Const(const String & value):
        number{(storage_number_t)(double)value},
        string{new String(value)},
        type{Type::STRING} {}

    Const(const Const & origin):
        number{origin.number},
        type{origin.type},
        string{origin.type == Type::STRING ? new String(*origin.string) : nullptr} {}

    make_bool_op(<=);
    make_bool_op(>=);
    make_bool_op(<);
    make_bool_op(>);
    make_bool_op(==);

    make_math_bin_op(-);
    make_math_bin_op(+);
    make_math_bin_op(/);
    make_math_bin_op(*);

    bool operator!() const { return !number; }
    constexpr operator storage_number_t() const { return number; }

    operator const wchar_t *() const {
        return type == Type::STRING ? string->data : getNumberStr();
    }

    wchar_t * restrict__ & getNumberStr() const {
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
        uint16_t size = useExpNotation ? swprintf(globalNumStrTmp, 325, NT_FORMAT_EXP, number)
                                       : swprintf(globalNumStrTmp, 325, NT_FORMAT, number);

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

    const wchar_t * restrict__ toString() const {
        return type == Type::STRING ? string->data : getNumberStr();
    }

    constexpr ~Const() {
        if (!numberStrTmp && !string) return;

        if (numberStrTmp) {
            free(numberStrTmp);
        }

        if (string) {
            // wprintf(L"wrp addr: %x, c addr: %x\n", string, this);
            delete string;
        }
    }
};

class ArgT;

class Var: public Const {
public:
    using Const::Const;

    Var(const ArgT & origin): Const((const Const &)origin) {}
    Var(const Var & origin): Const((const Const &)origin) {}

    Var & operator=(const Const & origin) {
        number = origin.number;
        type = origin.type;

        if (type == Type::NUMBER) {
            return *this;
        }

        if (string) {
            *string = *origin.string;
        } else {
            string = new String(*origin.string);
        }
        return *this;
    }

    Var & operator=(const Var & origin) { return operator=((const Const &)origin); }

    Var & operator=(const ArgT & origin) { return operator=((const Const &)origin); }

    Var & operator=(const ConstInitData & data) {
        number = data.number;
        type = data.type;

        if (data.str) {
            if (string) {
                *string = data.str;
            } else {
                string = new String(data.str);
            }
        }
        return *this;
    }

    Var & operator=(OneOfT<const wchar_t> auto * restrict__ value) {
        if (string) {
            *string = value;
        } else {
            string = new String(value);
        }

        number = (double)*string;
        type = Type::STRING;
        return *this;
    }

    Var & operator=(const String & value) {
        if (string) {
            *string = value;
        } else {
            string = new String(value);
        }

        number = (double)value;
        type = Type::STRING;
        return *this;
    }
    Var & operator=(String && value) {
        if (string) {
            *string = value;
        } else {
            string = new String(value);
        }

        number = (double)value;
        type = Type::STRING;
        return *this;
    }

    Var & operator=(NumberT auto value) {
        number = value;
        type = Type::NUMBER;
        return *this;
    }

    Var & operator++(int) {
        number++;
        type = Type::NUMBER;
        return *this;
    }

    Var & operator--(int) {
        number--;
        type = Type::NUMBER;
        return *this;
    }

    Var & operator+=(double value) {
        number += value;
        type = Type::NUMBER;
        return *this;
    }
};


class ArgT: public Const {
protected:
    bool stringCleaning = false;

public:
    ArgT(const ConstInitData & data):
        Const(data.number, data.str ? new String(data.str) : nullptr, data.type),
        stringCleaning{!!data.str} {}

    constexpr ArgT(NumberT auto value): Const(value) {}

    constexpr ArgT(NumberT auto num, String * str, Type _type = Type::NUMBER):
        Const(num, str, _type) {}

    ArgT(OneOfT<const wchar_t> auto * restrict__ value):
        Const(0, new String(wcslen(value), value, true), Type::STRING),
        stringCleaning{true} {
        number = (double)*string;
    }
    ArgT(String && value): Const((double)value, &value, Type::STRING) {}
    ArgT(const String & value): Const((double)value, (String *)&value, Type::STRING) {}

    ArgT(const Const & origin):
        Const(
            origin.number,
            origin.type == Type::STRING ? new String(*origin.string) : nullptr,
            origin.type
        ),
        stringCleaning{origin.type == Type::STRING}
    {
        // own the num-to-str cache,
        // so we don't have to generate a new one neither to depend on the origin's state
        if (origin.numberStrTmp) {
            numberStrTmp = origin.numberStrTmp;
            numberStrSize = origin.numberStrSize;
            numberStrLen = origin.numberStrLen;
            previousNumber = origin.previousNumber;
            origin.numberStrTmp = nullptr;
            origin.numberStrSize = 0;
        }
    }

    ArgT(const ArgT & origin):
        Const(origin.number, origin.string, origin.type)
    {
        if (origin.numberStrTmp) {
            numberStrTmp = origin.numberStrTmp;
            numberStrSize = origin.numberStrSize;
            numberStrLen = origin.numberStrLen;
            previousNumber = origin.previousNumber;
            origin.numberStrTmp = nullptr;
            origin.numberStrSize = 0;
        }
    }

    constexpr ~ArgT() {
        if (!stringCleaning) string = nullptr;
    }
};

using Arg = const ArgT &;

#endif
