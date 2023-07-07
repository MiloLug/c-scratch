#include "time.h"

#include <chrono>
#include <ctime>

namespace CSTime {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::nanoseconds;
    using std::chrono::days;
    using std::chrono::hh_mm_ss;
    using std::chrono::weekday;
    using std::chrono::year_month_day;
    using std::chrono::system_clock;

    template<typename SrcT, typename DstT>
    auto castTimePoint(const auto tp) {
        static const auto srcNow = SrcT::now();
        static const auto dstNow = DstT::now();
        return dstNow + (tp - srcNow);
    }

    /**** TimeZone ****/

    TimeZone::TimeZone() {
        tzset();

        const std::time_t someTime = 60 * 60 * 100;
        const std::tm localTime = *std::localtime(&someTime);
        const std::tm globalTime = *std::gmtime(&someTime);

        int32_t hDiff, mDiff, sDiff;

        if (localTime.tm_yday > globalTime.tm_yday) {
            hDiff = localTime.tm_hour + (24 - globalTime.tm_hour);
            mDiff = localTime.tm_min - globalTime.tm_min;
        } else if (localTime.tm_yday < globalTime.tm_yday) {
            hDiff = - (globalTime.tm_hour + (24 - localTime.tm_hour));
            mDiff = globalTime.tm_min - localTime.tm_min;
        } else {
            hDiff = localTime.tm_hour - globalTime.tm_hour;
            mDiff = localTime.tm_min - globalTime.tm_min;
        }

        sDiff = hDiff * 60 * 60 + mDiff * 60;
        if ((isLocalOffsetNegative = sDiff < 0)) {
            sDiff = -sDiff;
        }
        localOffset = seconds{sDiff};
    }

    time_point TimeZone::toLocal(const time_point timePoint) const {
        return isLocalOffsetNegative ? timePoint - localOffset : timePoint + localOffset;
    }

    /**** ZonedTime ****/

    ZonedTime::ZonedTime(const TimeZone * _tz, auto _timePoint): tz{_tz}, timePoint{_timePoint} {}

    time_point ZonedTime::getLocalTime() const {
        return tz->toLocal(timePoint);
    }
    time_point ZonedTime::getSystemTime() const {
        return timePoint;
    }

    /**** Time ****/

    const TimeZone Time::currentZone = TimeZone();

    const auto y2kTimePoint = time_point{nanoseconds{Time::sToNS(946684800)}};

    constexpr int64_t toNS(auto t) { return duration_cast<nanoseconds>(t).count(); }
    constexpr double toDays(auto t) { return duration<double, days::period>(t).count(); }
    static inline auto zonedNow() {
        return ZonedTime{&Time::currentZone, default_clock::now()};
    }

    const int64_t Time::y2kStartTime = toNS(y2kTimePoint.time_since_epoch());
    const int64_t Time::programStartTime = toNS(zonedNow().getLocalTime().time_since_epoch());

    Time::Time() {
        sync();
        resetTimer();
    }

    void Time::sync() const volatile {
        const auto zonedTime = zonedNow();
        const auto localTime = zonedTime.getLocalTime();
        const auto localTimeSystem = castTimePoint<default_clock, system_clock>(localTime);
        const auto localDay = std::chrono::floor<days>(localTimeSystem);

        year_month_day ymd{localDay};
        weekday wd{localDay};
        hh_mm_ss hms{std::chrono::floor<seconds>(localTimeSystem - localDay)};

        currentTime = toNS(localTime.time_since_epoch());
        programTime = currentTime - programStartTime;
        year = (int)ymd.year();
        month = (unsigned int)ymd.month();
        date = (unsigned int)ymd.day();
        weekDay = wd.c_encoding() + 1;  // USA goofy week with Sunday as the 1st day
        hour = hms.hours().count();
        minute = hms.minutes().count();
        second = hms.seconds().count();

        const auto timeSinceY2k = zonedTime.getSystemTime() - y2kTimePoint;
        y2kTime = toNS(timeSinceY2k);
        y2kDays = toDays(timeSinceY2k);
    }
}

const volatile CSTime::Time csTime;
