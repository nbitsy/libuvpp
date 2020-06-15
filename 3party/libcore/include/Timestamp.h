
#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_

#include <algorithm>
#include <ctime>

namespace XSpace
{

class Timestamp
{
public:
    typedef long long Int64;
    // Difference between two TimeVal values in microseconds.
    typedef Int64 TimeDiff;
    // Monotonic UTC time value in microsecond resolution, with base time midnight, January 1, 1970.
    typedef Int64 TimeVal;
    typedef Int64 UtcTimeVal;

    static const TimeVal TIMEVAL_MIN;
    static const TimeVal TIMEVAL_MAX;

public:
    Timestamp();
    Timestamp(TimeVal timeval);
    Timestamp(const Timestamp &other);
    Timestamp &operator=(TimeVal timeval);
    Timestamp &operator=(const Timestamp &other);

    TimeDiff Elapsed() const;
    bool IsElapsed(TimeDiff interval) const;

    bool operator<(const Timestamp &other) const;
    bool operator<=(const Timestamp &other) const;
    bool operator>(const Timestamp &other) const;
    bool operator>=(const Timestamp &other) const;
    bool operator==(const Timestamp &other) const;
    bool operator!=(const Timestamp &other) const;

    Timestamp operator+(TimeDiff timediff);
    Timestamp operator-(TimeDiff timediff);

    Timestamp operator+(const Timestamp &timediff);
    TimeDiff operator-(const Timestamp &timediff);

    Timestamp &operator+=(TimeDiff timediff);
    Timestamp &operator-=(TimeDiff timediff);

    Timestamp &operator+=(const Timestamp &timediff);
    Timestamp &operator-=(const Timestamp &timediff);

    Timestamp Now() const;
    TimeVal Raw() const;
    TimeVal Ticks() const;
    void Swap(Timestamp &other);
    // delay*1000*1000 = second, Micro Seconds
    Timestamp& UpdateMicrosecond(TimeDiff delay = 0);
    // delay*1000 = second, millisecond
    Timestamp& UpdateMillisecond(TimeDiff delay = 0);
    // delay = second, millisecond
    Timestamp& UpdateSecond(TimeDiff delay = 0);

    //inline Timestamp& Update(TimeDiff delay = 0) { return UpdateMillisecond(delay); }
    UtcTimeVal ToUtcTime() const;
    std::time_t EpochTime() const;

    static Timestamp FromUtcTime(UtcTimeVal timeval);
    static Timestamp FromEpochTime(TimeVal timeval);
    static TimeDiff Resolution();

private:
    TimeVal _tv;
};

inline Timestamp &Timestamp::operator=(TimeVal timeval)
{
    _tv = timeval;
    return *this;
}

inline Timestamp &Timestamp::operator=(const Timestamp &other)
{
    _tv = other._tv;
    return *this;
}

inline Timestamp::TimeDiff Timestamp::Resolution()
{
    return 1000000;
}

inline Timestamp::TimeDiff Timestamp::Elapsed() const
{
    Timestamp now;
    return now - *this;
}

inline bool Timestamp::IsElapsed(TimeDiff interval) const
{
    Timestamp now;
    if (now._tv - _tv >= interval)
        return true;
    return false;
}

inline bool Timestamp::operator<(const Timestamp &other) const
{
    return _tv < other._tv;
}
inline bool Timestamp::operator<=(const Timestamp &other) const
{
    return _tv <= other._tv;
}
inline bool Timestamp::operator>(const Timestamp &other) const
{
    return _tv > other._tv;
}
inline bool Timestamp::operator>=(const Timestamp &other) const
{
    return _tv >= other._tv;
}
inline bool Timestamp::operator==(const Timestamp &other) const
{
    return _tv == other._tv;
}
inline bool Timestamp::operator!=(const Timestamp &other) const
{
    return _tv != other._tv;
}

inline Timestamp Timestamp::operator+(TimeDiff timediff)
{
    return timediff + _tv;
}

inline Timestamp Timestamp::operator-(TimeDiff timediff)
{
    return _tv - timediff;
}

inline Timestamp Timestamp::operator+(const Timestamp &other)
{
    return _tv + other._tv;
}

inline Timestamp::TimeDiff Timestamp::operator-(const Timestamp &other)
{
    return _tv - other._tv;
}

inline Timestamp &Timestamp::operator+=(TimeDiff timediff)
{
    _tv += timediff;
    return *this;
}

inline Timestamp &Timestamp::operator-=(TimeDiff timediff)
{
    _tv -= timediff;
    return *this;
}

inline Timestamp &Timestamp::operator+=(const Timestamp &other)
{
    _tv += other._tv;
    return *this;
}

inline Timestamp &Timestamp::operator-=(const Timestamp &other)
{
    _tv -= other._tv;
    return *this;
}

inline Timestamp Timestamp::Now() const
{
    return Timestamp();
}

inline Timestamp::TimeVal Timestamp::Raw() const
{
    return _tv;
}

inline Timestamp::TimeVal Timestamp::Ticks() const
{
    return _tv;
}

inline void Timestamp::Swap(Timestamp &other)
{
    std::swap(_tv, other._tv);
}

inline Timestamp::UtcTimeVal Timestamp::ToUtcTime() const
{
    return _tv * 10 + (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
}

inline std::time_t Timestamp::EpochTime() const
{
    return std::time_t(_tv / Resolution());
}

} // namespace XSpace

#endif // _TIMESTAMP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
