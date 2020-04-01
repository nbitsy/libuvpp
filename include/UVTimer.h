
#ifndef _UVTIMER_H_
#define _UVTIMER_H_

#include "Timestamp.h"
#include "TypeTraits.h"
#include "UVHandle.h"

namespace XSpace
{

class UVLoop;

class UVTimer : public UVHandle
{
public:
    UV_CREATE_HANDLE(UVTimer)

public:
    virtual ~UVTimer();
    // timeout 第一次开始时的延迟时间
    // repeat 下一次的回调的间隔时间,ms
    // ticks 几个tick之后停止
    bool Start(uint64_t repeat = 5, uint64_t timeout = 0, long long ticks = -1);
    bool Again();
    void Stop(bool close = false);

    void SetRepeat(unsigned long long repeat);
    unsigned long long GetRepeat() const;

    void OnClosed() OVERRIDE;

    virtual void Tick(const Timestamp *now);

protected:
    UVTimer(const std::weak_ptr<UVLoop> &loop, long long ticks = -1);

public:
    long long Ticks;

private:
    Timestamp _timestamp;
};

} // namespace XSpace

#endif // _UVTIMER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
