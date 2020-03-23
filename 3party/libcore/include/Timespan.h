
#ifndef _TIMESPAN_H_
#define _TIMESPAN_H_

#include "Timestamp.h"

namespace XSpace
{

class Timespan
{
public:
    typedef Timestamp::TimeDiff TimeDiff;

public:
    Timespan();
    Timespan(TimeDiff microseconds);
    Timespan(long seconds, long microseconds);
    Timespan(int days, int hours, int minutes, long seconds, long microseconds);
    Timespan(const Timespan &other);
    ~Timespan();

    Timespan &operator=(TimeDiff microseconds);
    Timespan &assign(long seconds, long microseconds);
    Timespan &assign(int days, int hours, int minutes, long seconds, long microseconds);
    Timespan &operator=(const Timespan &other);
    void swap(Timespan &other);

    bool operator==(const Timespan &ts) const;
    bool operator!=(const Timespan &ts) const;
    bool operator>(const Timespan &ts) const;
    bool operator>=(const Timespan &ts) const;
    bool operator<(const Timespan &ts) const;
    bool operator<=(const Timespan &ts) const;

    bool operator==(TimeDiff microSeconds) const;
    bool operator!=(TimeDiff microSeconds) const;
    bool operator>(TimeDiff microSeconds) const;
    bool operator>=(TimeDiff microSeconds) const;
    bool operator<(TimeDiff microSeconds) const;
    bool operator<=(TimeDiff microSeconds) const;

    Timespan operator+(const Timespan &d) const;
    Timespan operator-(const Timespan &d) const;
    Timespan &operator+=(const Timespan &d);
    Timespan &operator-=(const Timespan &d);

    Timespan operator+(TimeDiff microSeconds) const;
    Timespan operator-(TimeDiff microSeconds) const;
    Timespan &operator+=(TimeDiff microSeconds);
    Timespan &operator-=(TimeDiff microSeconds);

    int days() const;
    int hours() const;
    int totalHours() const;
    int minutes() const;
    int totalMinutes() const;
    int seconds() const;
    int totalSeconds() const;
    int milliseconds() const;
    TimeDiff totalMilliseconds() const;
    int microseconds() const;
    int useconds() const;
    TimeDiff totalMicroseconds() const;

    std::string toString() const;

public:
    static const TimeDiff MILLISECONDS;
    static const TimeDiff SECONDS;
    static const TimeDiff MINUTES;
    static const TimeDiff HOURS;
    static const TimeDiff DAYS;

private:
    TimeDiff _span;
};

inline bool Timespan::operator==(const Timespan &ts) const
{
    return ts._span == _span;
}
inline bool Timespan::operator!=(const Timespan &ts) const
{
    return ts._span != _span;
}
inline bool Timespan::operator>(const Timespan &ts) const
{
    return _span > ts._span;
}
inline bool Timespan::operator>=(const Timespan &ts) const
{
    return _span >= ts._span;
}
inline bool Timespan::operator<(const Timespan &ts) const
{
    return _span < ts._span;
}
inline bool Timespan::operator<=(const Timespan &ts) const
{
    return _span <= ts._span;
}

inline bool Timespan::operator==(TimeDiff microSeconds) const
{
    return _span == microSeconds;
}
inline bool Timespan::operator!=(TimeDiff microSeconds) const
{
    return _span != microSeconds;
}
inline bool Timespan::operator>(TimeDiff microSeconds) const
{
    return _span > microSeconds;
}
inline bool Timespan::operator>=(TimeDiff microSeconds) const
{
    return _span >= microSeconds;
}
inline bool Timespan::operator<(TimeDiff microSeconds) const
{
    return _span < microSeconds;
}
inline bool Timespan::operator<=(TimeDiff microSeconds) const
{
    return _span <= microSeconds;
}

inline Timespan Timespan::operator+(const Timespan &d) const
{
    return _span + d._span;
}
inline Timespan Timespan::operator-(const Timespan &d) const
{
    return _span - d._span;
}
inline Timespan &Timespan::operator+=(const Timespan &d)
{
    _span += d._span;
    return *this;
}
inline Timespan &Timespan::operator-=(const Timespan &d)
{
    _span -= d._span;
    return *this;
}

inline Timespan Timespan::operator+(TimeDiff microSeconds) const
{
    return _span + microSeconds;
}
inline Timespan Timespan::operator-(TimeDiff microSeconds) const
{
    return _span - microSeconds;
}
inline Timespan &Timespan::operator+=(TimeDiff microSeconds)
{
    _span += microSeconds;
    return *this;
}
inline Timespan &Timespan::operator-=(TimeDiff microSeconds)
{
    _span -= microSeconds;
    return *this;
}

inline void Timespan::swap(Timespan &other)
{
    std::swap(_span, other._span);
}

inline void swap(Timespan &s1, Timespan &s2)
{
    s1.swap(s2);
}

inline int Timespan::days() const
{
    return (int)(_span / DAYS);
}
inline int Timespan::hours() const
{
    return (int)((_span / HOURS) % 24);
}
inline int Timespan::totalHours() const
{
    return (int)(_span / HOURS);
}
inline int Timespan::minutes() const
{
    return (int)((_span / MINUTES) % 60);
}
inline int Timespan::totalMinutes() const
{
    return (int)(_span / MINUTES);
}
inline int Timespan::seconds() const
{
    return (int)((_span / SECONDS) % 60);
}
inline int Timespan::totalSeconds() const
{
    return (int)(_span / SECONDS);
}
inline int Timespan::milliseconds() const
{
    return (int)((_span / MILLISECONDS) % 1000);
}
inline Timespan::TimeDiff Timespan::totalMilliseconds() const
{
    return (TimeDiff)(_span / MILLISECONDS);
}
inline int Timespan::microseconds() const
{
    return (int)(_span % 1000);
}
inline int Timespan::useconds() const
{
    return int(_span % 1000000);
}
inline Timespan::TimeDiff Timespan::totalMicroseconds() const
{
    return _span;
}

} // namespace XSpace

#endif // _TIMESPAN_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
