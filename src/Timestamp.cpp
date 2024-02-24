
#include "Config.h"
#include <limits>
#if defined(UNIXLIKE)
#include <time.h>
#include <unistd.h>
#if defined(VXWORKS)
#include <timers.h>
#else
#include <sys/time.h>
#include <sys/times.h>
#endif
#elif defined(WINDOWS)
#if defined(_WIN32_WCE)
#include <cmath>
#endif
#endif
#include "Timestamp.h"

namespace XNode
{

const Timestamp::TimeVal TIMEVAL_MIN = std::numeric_limits<Timestamp::TimeVal>::min();
const Timestamp::TimeVal TIMEVAL_MAX = std::numeric_limits<Timestamp::TimeVal>::max();

Timestamp::Timestamp()
{
    Update();
}

Timestamp::Timestamp(const Timestamp &other) : _tv(other._tv)
{
}

Timestamp::Timestamp(TimeVal timeval) : _tv(timeval)
{
}

void Timestamp::Update()
{
#if defined(WINDOWS)
    FILETIME ft;
#if defined(_WIN32_WCE) && defined(WINCE_TIMESTAMP_HACK)
    GetSystemTimeAsFileTimeWithMillisecondResolution(&ft);
#else
    GetSystemTimeAsFileTime(&ft);
#endif
    ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
    epoch.LowPart = 0xD53E8000;
    epoch.HighPart = 0x019DB1DE;

    ULARGE_INTEGER ts;
    ts.LowPart = ft.dwLowDateTime;
    ts.HighPart = ft.dwHighDateTime;
    ts.QuadPart -= epoch.QuadPart;
    _ts = ts.QuadPart / 10;
#elif defined(LOCK_GETTIME)
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts))
        return;
    _ts = TimeVal(ts.tv_sec) * resolution() + ts.tv_nsec / 1000;
#else
    struct timeval tv;
    if (gettimeofday(&tv, NULL))
        return;

    _tv = TimeVal(tv.tv_sec) * Resolution() + tv.tv_usec;
#endif
}

Timestamp Timestamp::FromUtcTime(UtcTimeVal timeval) 
{
    timeval -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
    timeval /= 10;
    return timeval;
}

Timestamp Timestamp::FromEpochTime(TimeVal timeval)
{
    return Timestamp(TimeVal(timeval) * Resolution());
}

} // namespace XNode

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
