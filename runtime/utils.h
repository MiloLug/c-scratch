#ifndef CSCRATCH_UTILS_H
#define CSCRATCH_UTILS_H

#include <cstdint>
#include <ctime>

#define SWAP(a, b)                                                                                 \
    ({                                                                                             \
        auto __tmp = (b);                                                                          \
        b = a;                                                                                     \
        a = __tmp;                                                                                 \
    })

#if defined(__clang__)
    #define restrict__ __restrict
#elif defined(__GNUC__) || defined(__GNUG__)
    #define restrict__ __restrict__
#elif defined(_MSC_VER)
    #define restrict__ __restrict
#else
    #define restrict__ /* no-op */
#endif


#ifndef force_inline__
    #if defined(_MSC_VER)
        #define force_inline__ __forceinline
    #elif ((defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__))
        #define force_inline__ __attribute__((always_inline)) inline
    #else
        #define force_inline__ inline
    #endif
#endif


static constexpr uint64_t fastHash(const wchar_t * str) {
    constexpr uint64_t maxOffset = 1LL << 61;
    uint64_t res = 0;
    wchar_t c = 0;
    while ((c = *(str++)) != L'\0') {
        res = res >= maxOffset ? (res >> 7) ^ c : (res << 3) ^ c;
    }

    return res;
}

/*
* Returns the string, hashed with `fastHash` from runtime/utils.h
*/
static constexpr uint64_t operator""_H(const wchar_t * str, size_t) { return fastHash(str); }


/**** Templates Stuff ****/

template<typename T>
struct remove_deepest_const_impl {
    typedef T type;
};
template<typename T>
struct remove_deepest_const_impl<const T> {
    typedef T type;
};
template<typename T>
struct remove_deepest_const_impl<T *> {
    typedef typename remove_deepest_const_impl<T>::type * type;
};
template<typename T>
struct remove_deepest_const_impl<T * const> {
    typedef typename remove_deepest_const_impl<T>::type * const type;
};
template<typename T>
struct remove_deepest_const_impl<T &> {
    typedef typename remove_deepest_const_impl<T>::type & type;
};
template<typename T>
struct remove_deepest_const_impl<T &&> {
    typedef typename remove_deepest_const_impl<T>::type && type;
};
template<typename T>
using remove_deepest_const = typename remove_deepest_const_impl<T>::type;


template<typename T>
concept Number = std::is_floating_point_v<T> || std::is_integral_v<T>;

template<typename T, typename U>
concept MaybeConst = std::is_same_v<U, remove_deepest_const<T>>;


#endif
