#ifndef CSCRATCH_TIME_H
#define CSCRATCH_TIME_H

#include "utils.h"

#include <chrono>
#include <ctime>


namespace CSTime {
    using std::chrono::duration;
    using std::chrono::seconds;
    using default_clock = std::chrono::high_resolution_clock;
    using time_point = default_clock::time_point;
    using clock_duration = default_clock::duration;

    struct TimeZone {
        clock_duration localOffset;
        bool isLocalOffsetNegative;

        TimeZone();
        time_point toLocal(const time_point timePoint) const;
    };

    struct ZonedTime {
        const TimeZone * const tz;
        const time_point timePoint;

        ZonedTime(const TimeZone * _tz, auto _timePoint);

        auto getLocalTime() const;
        auto getSystemTime() const;
    };

    struct Time {
        static const TimeZone currentZone;
        
        // timestamp of the year 2000 in ns
        static const int64_t y2kStartTime;
        static const int64_t programStartTime;

        // UNIX timestamp in ns
        volatile mutable int64_t currentTime;

        // Time from the program start in ns
        volatile mutable int64_t programTime;

        // Current timer's time in nanosecs
        mutable int64_t timerStartTime;

        // timestamp from the year 2000 in ns
        volatile mutable int64_t y2kTime;
        // days from the year 2000
        volatile mutable double y2kDays;

        volatile mutable int16_t year;
        volatile mutable int8_t month;
        volatile mutable int8_t date;
        // 1-based week day
        volatile mutable int8_t weekDay;
        // 24-based hour of current day
        volatile mutable int8_t hour;
        volatile mutable int8_t minute;
        volatile mutable int8_t second;

        static constexpr int64_t sToNS(double x) { return x * 1e9; }
        static constexpr double nsToS(int64_t x) { return (double)x / 1e9; }

        Time();

        void sync() const volatile;

        // Returns current timer's time in seconds
        force_inline__ double timer() const volatile { return nsToS(programTime - timerStartTime); }
        force_inline__ void resetTimer() const volatile { timerStartTime = programTime; }
    };
}

extern const volatile CSTime::Time csTime;
using CSTime::Time;


#endif
