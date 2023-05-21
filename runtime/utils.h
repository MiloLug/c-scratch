#ifndef CSCRATCH_UTILS_H
#define CSCRATCH_UTILS_H

#include <cstdint>
#include <functional>
#include <type_traits>
#include <memory>


#if defined(COMPILER_CLANG)
    #define restrict__ __restrict
#elif defined(COMPILER_GCC)
    #define restrict__ __restrict__
#elif defined(COMPILER_MSVC)
    #define restrict__ __restrict
#else
    #define restrict__ /* no-op */
#endif


#ifndef force_inline__
    #if defined(COMPILER_MSVC)
        #define force_inline__ __forceinline
    #elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
        #define force_inline__ __attribute__((always_inline)) inline
    #else
        #define force_inline__ inline
    #endif
#endif


//template <typename T>
//class UniquePtr: std::unique_ptr<T> {
//    constexpr UniquePtr(T * ptr) {
//
//    }
//};


static constexpr uint64_t fastHash(const wchar_t * str) {
    constexpr uint64_t maxOffset = 32;
    uint64_t res = 0;
    wchar_t c = 0;
    uint8_t i;
    uint8_t j;
    while ((c = *str) != L'\0') {
        j = 1;
        i = 0;
        do {
            if (i > maxOffset) i = j++;
            res = res ^ ((uint64_t)c << i);
            i += 4;
        } while ((c = *(++str)) != L'\0' && j < maxOffset);
    }

    return res;
}


/*
* Returns the string, hashed with `fastHash` from runtime/utils.h
*/
static constexpr uint64_t operator""_H(const wchar_t * str, size_t) { return fastHash(str); }


/**** Templates Stuff ****/

// remove_deepest_const

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


// general types

template<typename T>
concept NumberT = std::is_floating_point_v<T> || std::is_integral_v<T>;

template<typename T, typename U>
concept MaybeConstT = std::is_same_v<U, remove_deepest_const<T>>;

template<typename T, typename... U>
concept OneOfT = (std::is_same_v<U, T> || ...);

template<typename T, typename... U>
concept OneOfConvertibleT = (std::is_convertible_v<T, U> || ...);

template<typename...>
using t_exists = void;

template<typename Callable>
using return_type_of_t =
    typename std::invoke_result_t<decltype(std::function{std::declval<Callable>()})>;


// map utils

template<typename T>
using mapped_type_t = typename std::remove_cvref_t<T>::mapped_type;

template<typename T>
using key_type_t = typename std::remove_cvref_t<T>::key_type;


template<class T>
concept MappingT = requires(T t, typename T::key_type key) {
    typename T::mapped_type;
    {
        t.find(key)
    } -> std::same_as<typename T::iterator>;
};

template<class T>
concept SetT =
    !requires(T) { typename T::mapped_type; } && requires(T t, typename T::key_type key) {
        {
            t.find(key)
        } -> std::same_as<typename T::iterator>;
    };


template<MappingT T>
static constexpr auto findOr(T & container, auto && value, auto && _default) {
    auto found = container.find(value);
    return found != container.end() ? found->second : (mapped_type_t<T> &&)_default;
}

template<SetT T>
static constexpr auto findOr(T & container, auto && value, auto && _default) {
    auto found = container.find(value);
    return found != container.end() ? *found : (key_type_t<T> &&)_default;
}

#endif
