#ifndef CSCRATCH_ACTIONS_H
#define CSCRATCH_ACTIONS_H

#include "utils.h"

#include <cstdint>

/*
* Actions are so long to have parameters encoding possibility like
* `ACTION_KEYDOWN | <keyCode>` or `ACTION_MESSAGE | <messageIndex>`
*/
constexpr uint64_t ACTION_START = 0x1000'0000'0000'0000, ACTION_KEYDOWN = 0x2000'0000'0000'0000,
                   ACTION_MESSAGE = 0x3000'0000'0000'0000, ACTION_CLICK = 0x4000'0000'0000'0000;

/*
* This is how much bits action ID takes in the action specification.
* All other integer space is for the parameters.
*/
constexpr uint8_t ACTION_ID_SPACE = 8;
constexpr uint64_t ACTION_ID_MASK = 0xFF00'0000'0000'0000;


static constexpr uint64_t strToActionParam(const wchar_t * str) {
    return fastHash(str) & ~ACTION_ID_MASK;
}

/*
* Returns a hash of the string, ready to be used as an action parameter
*/
static constexpr uint64_t operator""_A(const wchar_t * str, size_t) {
    return strToActionParam(str);
}


#endif