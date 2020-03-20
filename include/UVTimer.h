
#ifndef _UVTIMER_H_
#define _UVTIMER_H_

#include "UVHandle.h"
#include "Timestamp.h"

namespace XSpace
{

class UVLoop;

class UVTimer : public UVHandle
{
public:
    static UVTimer *Create(UVLoop *loop)
    {
        return new UVTimer(loop); // TODO:
    }

    static void Destroy(UVTimer *timer)
    {
        if (NULL == timer)
            return;
        timer->Stop();
    }

    UVTimer(UVLoop *loop);
    virtual ~UVTimer();

public:
    // timeout: 第一次开始时的延迟时间
    // repeat: 下一次的回调的间隔时间,ms
    bool Start(uint64_t repeat = 5, uint64_t timeout = 0);
    void Stop();

    void SetRepeat(unsigned long long repeat);
    unsigned long long GetRepeat() const;

    void OnClosed();
    virtual void Tick(const Timestamp* now);
    void Release();

private:
    Timestamp _timestamp;
};

} // namespace XSpace

#endif // _UVTIMER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
