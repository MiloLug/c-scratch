/*
 * Originally written by Joel Yliluoma <joel.yliluoma@iki.fi>
 * http://en.wikipedia.org/wiki/Talk:Boyer%E2%80%93Moore_string_search_algorithm#Common_functions
 *
 * Copyright (c) 2008 Joel Yliluoma
 * Copyright (c) 2010 Hongli Lai
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "_bmh_search.h"

#include <climits>
#include <cstring>


namespace BMHSearch {
    /*
    * This function creates an occ table to be used by the search algorithms. 
    * It only needs to be created once per a needle to search.
    */
    const occTable_t createOccTable(const unsigned char * needle, const uint64_t needleLength) {
        occTable_t occ(
            UCHAR_MAX + 1, needleLength
        );  // initialize a table of UCHAR_MAX+1 elements to value needleLength

        /*
        * Populate it with the analysis of the needle
        * But ignoring the last letter
        */
        const uint64_t needleLengthMinus1 = needleLength - 1;
        for (uint64_t a = 0; a < needleLengthMinus1; ++a) occ[needle[a]] = needleLengthMinus1 - a;
        return occ;
    }

    /*
    * A Boyer-Moore-Horspool search algorithm.
    * If it finds the needle, it returns an offset to haystack from which
    * the needle was found. Otherwise, it returns haystackLength.
    */
    uint64_t searchInHorspool(
        const unsigned char * haystack,
        const uint64_t haystackLength,
        const occTable_t & occ,
        const unsigned char * needle,
        const uint64_t needleLength
    ) {
        // No need to check the haystack vs needle lengths or the needle length = 1 case.
        // It just won't be used in those cases

        const uint64_t needleLengthMinus1 = needleLength - 1;

        const unsigned char lastNeedleChar = needle[needleLengthMinus1];

        uint64_t haystackPosition = 0;
        while (haystackPosition <= haystackLength - needleLength) {
            const unsigned char occChar = haystack[haystackPosition + needleLengthMinus1];

            // The author modified this part. Original algorithm matches needle right-to-left.
            // This code calls memcmp() (usually matches left-to-right) after matching the last
            // character, thereby incorporating some ideas from
            // "Tuning the Boyer-Moore-Horspool String Searching Algorithm"
            // by Timo Raita, 1992.
            if (lastNeedleChar == occChar
                && std::memcmp(needle, haystack + haystackPosition, needleLengthMinus1) == 0)
            {
                return haystackPosition;
            }

            haystackPosition += occ[occChar];
        }
        return haystackLength;
    }
}
