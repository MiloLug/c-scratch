#ifndef CSCRATCH_STRING_H
#define CSCRATCH_STRING_H

#include "_str_to_num.h"
#include "runtime/utils.h"

#include <cstdint>
#include <cstring>
#include <memory>


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
    bool isWrapper = false;  // delete is forbidden in any case, force copying on `copy` etc
    uint64_t mSize = 0;  // memory taken in bytes
    wchar_t * mData = nullptr;
    uint64_t mLength = 0;  // number of chars
public:
    const uint64_t & size = mSize;
    const wchar_t * & data = const_cast<const wchar_t * &>(mData);
    const uint64_t & length = mLength;

    /*Create an empty string*/
    constexpr String(): mLength{0}, mData{(wchar_t *)emptyString}, mSize{1 << 2}, isWrapper{true} {}

    /*Create a string repeating the symbol from `sym`*/
    String(wchar_t sym, uint64_t _length):
        mLength{_length},
        mData{(wchar_t *)malloc((_length + 1) << 2)},
        mSize{(_length + 1) << 2} {
        if (_length == 1)
            mData[0] = sym;
        else
            while (_length > 0) mData[--_length] = sym;

        mData[mLength] = L'\0';
    }

    /*Wrap and existing string*/
    constexpr String(uint64_t _length, const wchar_t * _data, bool _isWrapper = false):
        mLength(_length),
        mData((wchar_t *)_data),
        mSize((_length + 1) << 2),
        isWrapper(_isWrapper) {}

    String(const wchar_t * restrict__ _data) {
        mLength = wcslen(_data);
        mSize = (mLength + 1) << 2;
        mData = (wchar_t *)malloc(mSize);
        memcpy(mData, _data, mSize);
    }
    String(String && origin):
        mLength{origin.mLength},
        mSize{origin.mSize},
        mData{origin.mData},
        isWrapper{origin.isWrapper} {
        origin.mData = nullptr;
    }
    String(const String & origin) {
        mLength = origin.mLength;
        mSize = origin.mSize;
        if (origin.mData) {
            mData = (wchar_t *)malloc(origin.mSize);
            memcpy(mData, origin.mData, origin.mSize);
        }
    }

    String & operator=(String && origin) {
        if (!isWrapper && mData) free(mData);

        mLength = origin.mLength;
        mSize = origin.mSize;
        mData = origin.mData;
        isWrapper = origin.isWrapper;
        origin.mData = nullptr;

        return *this;
    }
    String & operator=(const String & origin) {
        mLength = origin.mLength;
        mSize = origin.mSize;
        if (origin.mData) {
            if (isWrapper) mData = nullptr;

            mData = (wchar_t *)realloc((void *)mData, origin.mSize);
            memcpy(mData, origin.mData, origin.mSize);
        }
        isWrapper = false;

        return *this;
    }
    String & operator=(const wchar_t * value) {
        mLength = wcslen(value);
        mSize = (mLength + 1) << 2;
        mData = (wchar_t *)realloc((void *)mData, mSize);

        if (mData) memcpy(mData, value, mSize);

        return *this;
    }

    constexpr void clean() {
        if (mData && !isWrapper) free(mData);
    }

    constexpr operator const wchar_t *() const { return mData; }

    operator double() const { return strToNum(mData, mLength); }

    constexpr ~String() { clean(); }
};


#endif