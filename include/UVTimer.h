
#ifndef UVTIMER_H_
#define UVTIMER_H_

#include "UVHandle.h"
#include "Timestamp.h"

namespace XNode
{

class UVLoop;
class UVTime;

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

public:
    // timeout: 第一次开始时的延迟时间
    // repeat: 下一次的回调的间隔时间,ms
    bool Start(uint64_t repeat = 5, uint64_t timeout = 0);
    void Stop();

    void SetRepeat(unsigned long long repeat);
    unsigned long long GetRepeat() const;

    void OnClosed();
    virtual void Tick(Timestamp* now);
    void Release();

private:
    UVTimer(UVLoop *loop);
    ~UVTimer();

private:
    Timestamp _timestamp;
};

} // namespace XNode

#endif // UVTIMER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
