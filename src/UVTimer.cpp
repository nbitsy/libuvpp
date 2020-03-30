
#include "UVTimer.h"
#include "UVLoop.h"

namespace XSpace
{

UVTimer::UVTimer(const std::weak_ptr<UVLoop> &loop, long long ticks)
    : UVHandle(loop), Ticks(ticks)
{
    if (_loop.expired())
        return;

    auto l = _loop.lock();
    _handle = (uv_handle_t *)Allocator::Construct<uv_timer_t>();
    if (_handle != NULL && l->GetRawLoop<uv_loop_t>() != NULL)
    {
        uv_timer_init(l->GetRawLoop<uv_loop_t>(), GetHandle<uv_timer_t>());
        uv_handle_set_data(_handle, NULL);
    }
    DEBUG("Object @%p\n", this);
}

UVTimer::~UVTimer()
{
    if (_handle != NULL)
        Stop();

    DEBUG("Object @%p\n", this);
}

void __OnTimer(uv_timer_t *timer)
{
    UVData *uvdata = (UVData *)uv_handle_get_data((uv_handle_t *)timer);
    if (NULL == uvdata || NULL == uvdata->_self)
        return;

    UVTimer *t = uvdata->GetPtr<UVTimer>();
    if (NULL == t)
        return;

    Timestamp *timestamp = (Timestamp *)uvdata->_data;
    if (timestamp != NULL)
        timestamp->Update();

    t->Tick(timestamp);

    if (t->Ticks == 0)
        t->Stop();
    else if (t->Ticks > 0)
        --t->Ticks;
}

// timeout: 第一次开始时的延迟时间
// repeat: 下一次的回调的间隔时间,ms
bool UVTimer::Start(uint64_t repeat, uint64_t timeout, long long ticks)
{
    if (NULL == _handle || !_handle->flags)
        return false;

    if (ticks >= 0)
        Ticks = ticks;

    return !uv_timer_start(GetHandle<uv_timer_t>(), __OnTimer, timeout, repeat);
}

void UVTimer::Stop()
{
    if (NULL == _handle || !_handle->flags)
        return;

    uv_timer_stop(GetHandle<uv_timer_t>());
    Close();
}

void UVTimer::SetRepeat(unsigned long long repeat)
{
    if (NULL == _handle)
        return;

    uv_timer_set_repeat(GetHandle<uv_timer_t>(), repeat);
}

unsigned long long UVTimer::GetRepeat() const
{
    if (NULL == _handle)
        return 0;

    return uv_timer_get_repeat(GetHandle<uv_timer_t>());
}

void UVTimer::OnClosed()
{
    DEBUG("\n");
}

void UVTimer::Tick(const Timestamp *now)
{
    static int i = 0;

#if 0
    if (i++ > 3)
    {
        Stop();
        return;
    }
#endif

    if (now != NULL)
    {
        DEBUG("Now: %lld\n", now->Ticks());
    }
    else
    {
        DEBUG("\n");
    }
}

} // namespace XSpace

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
