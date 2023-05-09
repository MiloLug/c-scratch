#ifndef CSCRATCH_UTILS_H
#define CSCRATCH_UTILS_H

#include <ctime>
#include <cstdint>

#define SWAP(a, b) ({auto __tmp = (b); b = a; a = __tmp;})

#if defined(__clang__)
    #define restrict__ __restrict
#elif defined(__GNUC__) || defined(__GNUG__)
    #define restrict__ __restrict__
#elif defined(_MSC_VER)
    #define restrict__ __restrict
#else
    #define restrict__ /* no-op */
#endif


static constexpr uint64_t fastHash(const wchar_t * str) {
    constexpr uint64_t maxOffset = 1LL << 61;
    uint64_t res = 0;
    wchar_t c = 0;
    while((c = *(str++)) != L'\0') {
        res = res >= maxOffset
            ? (res >> 7) ^ c
            : (res << 3) ^ c;
    }

    return res;
}

/*
* Returns the string, hashed with `fastHash` from runtime/utils.h
*/
static constexpr uint64_t operator ""_H(const wchar_t * str, size_t) {
    return fastHash(str);
}


#endif
