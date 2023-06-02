#include "time.h"

#include <chrono>

using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::zoned_time;
using time_point = high_resolution_clock::time_point;
using std::chrono::current_zone;
using std::chrono::days;
using std::chrono::year_month_day;
using std::chrono::weekday;
using std::chrono::hh_mm_ss;
using std::chrono::time_zone;
using std::chrono::local_time;

const time_zone * Time::currentZone = current_zone();

const auto y2kTimePoint = time_point{nanoseconds{Time::sToNS(946684800)}};

constexpr int64_t toNS(auto t) {return duration_cast<nanoseconds>(t).count();}
constexpr double toDays(auto t) {return duration<double, days::period>(t).count();}
static inline auto zonedNow() {
    return zoned_time{
        Time::currentZone,
        high_resolution_clock::now()
    };
}

const int64_t Time::y2kStartTime = toNS(y2kTimePoint.time_since_epoch());
const int64_t Time::programStartTime = toNS(zonedNow().get_local_time().time_since_epoch());

Time::Time() {
    sync();
    resetTimer();
}

void Time::sync() const volatile {
    const auto zonedTime = zonedNow();
    const auto localTime = zonedTime.get_local_time();
    const auto localDay = std::chrono::floor<days>(localTime);

    year_month_day ymd{localDay};
    weekday wd{localDay};
    hh_mm_ss hms{std::chrono::floor<seconds>(localTime - localDay)};

    currentTime = toNS(localTime.time_since_epoch());
    programTime = currentTime - programStartTime;
    year = (int)ymd.year();
    month = (unsigned int)ymd.month();
    date = (unsigned int)ymd.day();
    weekDay = wd.c_encoding() + 1;  // USA goofy week with Sunday as the 1st day
    hour = hms.hours().count();
    minute = hms.minutes().count();
    second = hms.seconds().count();

    const auto y2kTmp = zonedTime.get_sys_time() - y2kTimePoint;
    y2kTime = toNS(y2kTmp);
    y2kDays = toDays(y2kTmp);
}

const volatile Time csTime;
