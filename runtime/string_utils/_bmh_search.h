#ifndef BMH_SEARCH_ALG_H
#define BMH_SEARCH_ALG_H

#include <cstdint>
#include <cwchar>
#include <vector>
#include <climits>


namespace BMHSearch {
    typedef std::vector<uint64_t> occTable_t;

    const occTable_t createOccTable(const unsigned char * needle, const uint64_t needleLength);
    uint64_t searchInHorspool(
        const unsigned char * haystack,
        const uint64_t haystackLength,
        const occTable_t & occ,
        const unsigned char * needle,
        const uint64_t needleLength
    );

    static inline int64_t findIn(
        const wchar_t * haystack,
        const uint64_t hsLength,
        const wchar_t * needle,
        const uint64_t ndLength
    ) {
        if (ndLength > hsLength) return hsLength;
        if (ndLength == 1) {
            const wchar_t * result = wmemchr(haystack, *needle, hsLength);
            return result ? uint64_t(result - haystack) : hsLength;
        }
        if (hsLength * ndLength < UCHAR_MAX) {
            for (uint16_t i = 0; i < hsLength - ndLength + 1; i++) {
                uint16_t j = 0;
                while (j < ndLength && haystack[i + j] == needle[j]) j++;
                if (j == ndLength) return i;
            }
            return hsLength;
        }

        auto occTable = createOccTable((const unsigned char *)needle, ndLength << 2);
        return (
            searchInHorspool(
                (const unsigned char *)haystack,
                hsLength << 2,
                occTable,
                (const unsigned char *)needle,
                ndLength << 2
            )
            >> 2
        );
    }
}


#endif