#ifndef CSCRATCH_STRING_H
#define CSCRATCH_STRING_H

#include "_str_to_num.h"
#include "runtime/utils.h"

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>


/*
 * This is just an inner container for wchar strings.
 * !!! It's generally not supposed to be stored on stack !!!
 * 
 * Some use cases:
 * - Returning value from a function. You can allocate a string and then use:
 *   return String(length, stringPointer, true)
 *   so the data won't be copied when assigning to a variable
 * 
 * - Storing a string as a pointer to String:
 *   String::create(L"test test test")
 */
class String {
protected:
    static constexpr const wchar_t * emptyString = L"";
    uint32_t mSize = 0;  // memory taken in bytes
    wchar_t * mData = nullptr;
    uint32_t mLength = 0;  // number of chars
    mutable bool mIsNumber = false;
public:
    bool isWrapper = false;  // delete is forbidden in any case, force copying on `copy` etc
    const uint32_t & size = mSize;
    const wchar_t * & data = const_cast<const wchar_t * &>(mData);
    const uint32_t & length = mLength;
    const bool & isNumber = mIsNumber;

    /*Create an empty string*/
    constexpr String(): mLength{0}, mData{(wchar_t *)emptyString}, mSize{1 << CHAR_SIZE_POWER}, isWrapper{true} {}

    /*Create a string repeating the symbol from `sym`*/
    String(wchar_t sym, uint32_t _length):
        mLength{_length},
        mData{(wchar_t *)malloc((_length + 1) << CHAR_SIZE_POWER)},
        mSize{(_length + 1) << CHAR_SIZE_POWER} {
        if (_length == 1)
            mData[0] = sym;
        else
            while (_length > 0) mData[--_length] = sym;

        mData[mLength] = L'\0';
    }

    /*Wrap and existing string*/
    constexpr String(uint32_t _length, const wchar_t * _data, bool _isWrapper = false):
        mLength(_length),
        mData((wchar_t *)_data),
        mSize((_length + 1) << CHAR_SIZE_POWER),
        isWrapper(_isWrapper) {}

    constexpr String(const wchar_t * _data, bool _isWrapper):
        mLength(std::char_traits<wchar_t>::length(_data)),
        mData((wchar_t *)_data),
        isWrapper(_isWrapper) {
            mSize = (mLength + 1) << CHAR_SIZE_POWER;
        }

    String(OneOfT<const wchar_t> auto * restrict__ _data) {
        mLength = wcslen(_data);
        mSize = (mLength + 1) << CHAR_SIZE_POWER;
        mData = (wchar_t *)malloc(mSize);
        memcpy(mData, _data, mSize);
    }
    String(String && origin):
        mLength{origin.mLength},
        mSize{origin.mSize},
        mData{origin.mData},
        isWrapper{origin.isWrapper},
        mIsNumber{origin.mIsNumber} {
        origin.mData = nullptr;
    }
    String(const String & origin) {
        mLength = origin.mLength;
        mSize = origin.mSize;
        mData = (wchar_t *)malloc(origin.mSize);
        memcpy(mData, origin.mData, origin.mSize);
        isWrapper = false;
        mIsNumber = origin.mIsNumber;
    }

    String & operator=(String && origin) noexcept {
        if (&origin == this) return *this;
        if (!isWrapper && mData) free(mData);

        mLength = origin.mLength;
        mSize = origin.mSize;
        mData = origin.mData;
        isWrapper = origin.isWrapper;
        mIsNumber = origin.mIsNumber;
        origin.mData = nullptr;

        return *this;
    }
    String & operator=(const String & origin) {
        if (&origin == this) return *this;
        mLength = origin.mLength;
        mSize = origin.mSize;
        mIsNumber = origin.mIsNumber;

        if (isWrapper) mData = nullptr;
        mData = (wchar_t *)realloc((void *)mData, origin.mSize);
        memcpy(mData, origin.mData, origin.mSize);
        isWrapper = false;

        return *this;
    }
    String & operator=(OneOfT<const wchar_t> auto * restrict__ value) {
        mLength = wcslen(value);
        mSize = (mLength + 1) << CHAR_SIZE_POWER;
        mIsNumber = false;

        if (isWrapper) mData = nullptr;
        mData = (wchar_t *)realloc((void *)mData, mSize);
        memcpy(mData, value, mSize);
        isWrapper = false;

        return *this;
    }

    constexpr operator const wchar_t *() const { return mData; }

    operator double() const {
        return strToNum(mData, mLength, &mIsNumber);
    }

    constexpr ~String() {
        if (mData && !isWrapper) {
            free(mData);
        }
    }
};


#endif
